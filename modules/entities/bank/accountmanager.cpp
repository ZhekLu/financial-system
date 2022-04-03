#include "accountmanager.h"

bool AccountManager::freeze_request(BankAccount *acc) {
  if (!update(acc))
    return false;
  acc->revert_frozen();
  if (send_request(acc,
                   Request(Request::FREEZE, acc->id, acc->id))) // TODO! user.id
    return true;
  acc->revert_frozen();
  return false;
}

bool AccountManager::withdraw_request(BankAccount *acc, size_t sum) {

  if (!update(acc))
    return false;
  acc->withdraw(sum);
  if (make_withdraw(acc, sum))
    return true;
  acc->top_up(sum);
  return false;
}

bool AccountManager::transfer_request(BankAccount *acc, size_t destination,
                                      size_t sum) {
  if (!update(acc))
    return false;
  acc->withdraw(sum);
  if (make_transaction(acc, destination, sum))
    return true;
  acc->top_up(sum);
  return false;
}

bool AccountManager::undo_transfer_request(size_t initiator, Transaction &t) {
  std::unique_ptr<BankAccount> sender(USER_DB->get_account(t.sender));
  std::unique_ptr<BankAccount> receiver(USER_DB->get_account(t.receiver));
  if (!sender || (t.type == Transaction::TRANSFER &&
                  (!receiver || !receiver->can_pay(t.amount))))
    return false;
  if (t.type == Transaction::TRANSFER)
    receiver->withdraw(t.amount);
  sender->top_up(t.amount);
  Transaction ut(t.receiver, t.sender, t.amount);
  ut.is_approved = send_request(sender.get(), receiver.get(),
                                Request(Request::UNDO, initiator, ut.get_id()));
  return send_transaction(ut);
}

bool AccountManager::add_account_request(BankAccount *acc) {
  Request r(Request::LOGIN_ACCOUNT, acc->id, acc->id); // TODO user.id
  r.is_approved = USER_DB->add_account(acc);
  return send_request(r);
}

bool AccountManager::update(BankAccount *acc) {
  if (check_valid(acc))
    return true;
  acc->set_balance(USER_DB->get_account_balance(acc->id));
  return check_valid(acc);
}

bool AccountManager::check_valid(BankAccount *acc) {
  return USER_DB->contains(*acc);
}

bool AccountManager::send_request(Request &r) {
  USER_DB->add_request(r);
  return r.is_approved;
}

bool AccountManager::send_request(BankAccount *acc, Request r) {
  r.is_approved = USER_DB->update(*acc);
  return send_request(r);
}

bool AccountManager::send_request(BankAccount *acc, BankAccount *sec,
                                  Request r) {
  r.is_approved = USER_DB->update(*acc);
  if (r.is_approved)
    r.is_approved = USER_DB->update(*sec);
  return send_request(r);
}

bool AccountManager::send_transaction(Transaction &t) {
  USER_DB->add_transaction(t);
  return t.is_approved;
}

bool AccountManager::make_transaction(BankAccount *acc, size_t dest,
                                      size_t sum) {
  std::unique_ptr<BankAccount> receiver(USER_DB->get_account(dest));
  if (!(bool)receiver)
    return false;

  receiver->top_up(sum);
  Transaction t(acc->id, receiver->id, sum);
  t.is_approved = send_request(
      acc, receiver.get(),
      Request(Request::TRANSFER, acc->id, t.get_id())); // TODO! user.id
  return send_transaction(t);
}

bool AccountManager::make_withdraw(BankAccount *acc, size_t sum) {
  Transaction t(acc->id, sum, true);
  t.is_approved = send_request(
      acc, Request(Request::WITHDRAW, acc->id, t.get_id())); // TODO! user.id
  return send_transaction(t);
}

AccountManager::AccountManager(IUser *user) : IHistoryManager(user) {
  AccountManager::update();
}

std::vector<QTableWidgetItem *> AccountManager::get_items() {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {

    QString item = requests[i]->get_info();
    if (transactions[i])
      item += transactions[i]->get_info();
    else
      qDebug() << "oaoa";
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

bool AccountManager::undo(size_t item_index) {
  Transaction *current = transactions[item_index].get();
  Request *r = requests[item_index].get();

  r->viewed = undo_transfer_request(user->get_id(), *current);
  if (r->viewed) {
    qDebug() << "request : " << USER_DB->update(*r);
    emit IHistoryManager::updated();
  }
  return r->viewed;
}

void AccountManager::update() {
  transactions.clear();
  requests.clear();
  requests = USER_DB->get_transfer_requests();
  for (auto &r : requests) {
    transactions.push_back(USER_DB->get_transaction(r->object_id));
  }
}
