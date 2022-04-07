#include "accountmanager.h"

bool AccountManager::freeze_request(size_t sender_id, size_t account_id,
                                    bool freeze) {
  std::unique_ptr<BankAccount> acc(USER_DB->get_account(account_id));
  if (!acc)
    return false;
  if (acc->is_frozen() == freeze)
    return true;
  acc->revert_frozen();
  return send_request(acc.get(),
                      Request(Request::FREEZE, sender_id, acc->get_id()));
}

bool AccountManager::withdraw_request(size_t sender_id, size_t account_id,
                                      size_t sum) {
  std::unique_ptr<BankAccount> acc(USER_DB->get_account(account_id));
  if (!acc || !acc->withdraw(sum))
    return false;
  return make_withdraw(sender_id, acc.get(), sum);
}

bool AccountManager::transfer_request(size_t sender_id, size_t account_id,
                                      size_t receiver_id, size_t sum) {
  std::unique_ptr<BankAccount> acc(USER_DB->get_account(account_id));
  if (!acc || !acc->withdraw(sum))
    return false;
  return make_transaction(sender_id, acc.get(), receiver_id, sum);
}

bool AccountManager::undo_transfer_request(size_t initiator, Transaction &t) {
  std::unique_ptr<BankAccount> sender(USER_DB->get_account(t.get_sender()));
  std::unique_ptr<BankAccount> receiver(USER_DB->get_account(t.get_receiver()));
  if (!sender || (t.get_type() == Transaction::TRANSFER &&
                  (!receiver || !receiver->can_pay(t.get_amount()))))
    return false;

  return t.get_type() == Transaction::TRANSFER
             ? make_undo_transaction(initiator, sender.get(), receiver.get(), t)
             : make_undo_withdraw(initiator, sender.get(), t);
}

bool AccountManager::make_undo_transaction(size_t initiator,
                                           BankAccount *sender,
                                           BankAccount *receiver,
                                           Transaction &original) {
  receiver->withdraw(original.get_amount());
  sender->top_up(original.get_amount());
  Transaction ut(original.get_receiver(), original.get_sender(),
                 original.get_amount());
  ut.set_approved(send_request(sender, receiver,
                               Request(Request::UNDO, initiator, ut.get_id())));
  return send_transaction(ut);
}

bool AccountManager::make_undo_withdraw(size_t initiator, BankAccount *acc,
                                        Transaction &original) {
  acc->top_up(original.get_amount());
  Transaction ut(acc->get_id(), original.get_amount(), false);
  ut.set_approved(
      send_request(acc, Request(Request::UNDO, initiator, ut.get_id())));
  return send_transaction(ut);
}

bool AccountManager::add_account_request(size_t sender_id, BankAccount *acc) {
  Request r(Request::LOGIN_ACCOUNT, sender_id, acc->get_id());
  r.set_approved(USER_DB->add_account(acc));
  return IHistoryManager::send_request(r);
}

bool AccountManager::send_request(BankAccount *acc, Request r) {
  r.set_approved(USER_DB->update(*acc));
  return IHistoryManager::send_request(r);
}

bool AccountManager::send_request(BankAccount *acc, BankAccount *sec,
                                  Request r) {
  r.set_approved(USER_DB->update(*acc));
  if (r.is_approved())
    r.set_approved(USER_DB->update(*sec));
  return IHistoryManager::send_request(r);
}

bool AccountManager::send_transaction(Transaction &t) {
  USER_DB->add_transaction(t);
  return t.is_approved();
}

bool AccountManager::make_transaction(size_t sender, BankAccount *acc,
                                      size_t dest, size_t sum) {
  std::unique_ptr<BankAccount> receiver(USER_DB->get_account(dest));
  if (!receiver)
    return false;

  receiver->top_up(sum);
  Transaction t(acc->get_id(), receiver->get_id(), sum);
  t.set_approved(send_request(acc, receiver.get(),
                              Request(Request::TRANSFER, sender, t.get_id())));
  return send_transaction(t);
}

bool AccountManager::make_withdraw(size_t sender, BankAccount *acc,
                                   size_t sum) {
  Transaction t(acc->get_id(), sum, true);
  t.set_approved(
      send_request(acc, Request(Request::WITHDRAW, sender, t.get_id())));
  return send_transaction(t);
}

AccountManager::AccountManager(IUser *user) : IHistoryManager(user) {
  connect(USER_DB, &DataBase::updated, this, &AccountManager::update_vars);
  AccountManager::update_vars();
}

std::vector<QTableWidgetItem *> AccountManager::get_items() const {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {

    QString item = requests[i]->get_info();
    if (transactions[i])
      item += transactions[i]->get_info();
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

bool AccountManager::mark(size_t item_index, bool verify) {
  if (verify)
    return true;
  std::unique_ptr<Transaction> current = std::move(transactions[item_index]);
  std::unique_ptr<Request> r = std::move(requests[item_index]);

  r->set_viewed(undo_transfer_request(user->get_id(), *current));
  if (r->is_viewed())
    qDebug() << "Request : " << USER_DB->update(*r);
  return r->is_viewed();
}

size_t AccountManager::get_selected(size_t index) const {
  return transactions[index]->get_id();
}

void AccountManager::update_vars() {
  transactions.clear();
  requests.clear();
  requests = USER_DB->get_transfer_requests();
  for (auto &r : requests) {
    transactions.push_back(USER_DB->get_transaction(r->get_object()));
  }
}
