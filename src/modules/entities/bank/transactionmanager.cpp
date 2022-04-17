#include "transactionmanager.h"

// Static

bool TransactionManager::withdraw_request(size_t sender_id, size_t account_id,
                                          size_t sum) {
  std::unique_ptr<BankAccount> acc(AccountManager::get_account(account_id));
  if (!acc || !acc->withdraw(sum))
    return false;
  return make_withdraw(sender_id, acc.get(), sum);
}

bool TransactionManager::transfer_request(size_t sender_id, size_t account_id,
                                          size_t receiver_account_id,
                                          size_t sum) {
  std::unique_ptr<BankAccount> sender(AccountManager::get_account(account_id));
  std::unique_ptr<BankAccount> receiver(
      AccountManager::get_account(receiver_account_id));
  if (!sender || !receiver)
    return false;
  return make_transaction(sender_id, sender.get(), receiver.get(), sum);
}

bool TransactionManager::entity_transfer_request(size_t sender_id,
                                                 size_t account_id,
                                                 size_t receiver_id,
                                                 size_t sum) {
  std::unique_ptr<BankAccount> sender(AccountManager::get_account(account_id));
  std::unique_ptr<BankAccount> receiver(
      AccountManager::get_account(receiver_id));
  if (!sender || !receiver)
    return false;
  return make_transaction_request(sender_id, sender.get(), receiver.get(), sum);
}

bool TransactionManager::undo_transfer(size_t initiator, Transaction &t) {
  std::unique_ptr<BankAccount> sender(
      AccountManager::get_account(t.get_sender()));
  std::unique_ptr<BankAccount> receiver(
      AccountManager::get_account(t.get_receiver()));
  if (!sender || (t.get_type() == Transaction::TRANSFER &&
                  (!receiver || !receiver->can_pay(t.get_amount()))))
    return false;

  if (!t.is_approved()) {
    //    return !IHistoryManager::send_request(Request)
    return false; // whaaaat is there needed
  }

  return t.get_type() == Transaction::TRANSFER
             ? make_undo_transaction(initiator, sender.get(), receiver.get(), t)
             : make_undo_withdraw(initiator, sender.get(), t);
}

bool TransactionManager::undo_transfer_request(size_t initiator,
                                               Request &original) {
  Request ur(Request::UNDO, initiator, original.get_id());
  ur.set_approved(true);

  original.set_viewed(IHistoryManager::send_request(ur));
  if (original.is_viewed())
    qDebug() << "Request : " << USER_DB->update(original);
  return original.is_viewed();
}

bool TransactionManager::make_undo_transaction(size_t initiator,
                                               BankAccount *sender,
                                               BankAccount *receiver,
                                               Transaction &original) {

  receiver->withdraw(original.get_amount());
  sender->top_up(original.get_amount());

  Transaction ut(original.get_receiver(), original.get_sender(),
                 original.get_amount());
  //  Request ur(Request::UNDO, initiator, ut.get_id());
  //  if (original.is_approved())
  ut.set_approved(send_request(sender, receiver,
                               Request(Request::UNDO, initiator, ut.get_id())));
  //  else
  //    IHistoryManager::send_request(ur);

  return send_transaction(ut);
}

bool TransactionManager::make_undo_withdraw(size_t initiator, BankAccount *acc,
                                            Transaction &original) {
  acc->top_up(original.get_amount());
  Transaction ut(acc->get_id(), original.get_amount(), false);
  ut.set_approved(
      send_request(acc, Request(Request::UNDO, initiator, ut.get_id())));
  return send_transaction(ut);
}

bool TransactionManager::send_request(BankAccount *acc, Request r) {
  r.set_approved(AccountManager::update(*acc));
  return IHistoryManager::send_request(r);
}

bool TransactionManager::send_request(BankAccount *acc, BankAccount *sec,
                                      Request r) {
  r.set_approved(AccountManager::update(*acc));
  if (r.is_approved())
    r.set_approved(AccountManager::update(*sec));
  return IHistoryManager::send_request(r);
}

bool TransactionManager::send_transaction(Transaction &t) {
  USER_DB->add_transaction(t);
  return t.is_approved();
}

bool TransactionManager::make_transaction(size_t sender, BankAccount *from,
                                          BankAccount *to, size_t sum) {
  // check if accounts are not in freeze
  if (!from->is_available() || !to->is_available())
    return false;

  // can pay?
  if (!from->withdraw(sum))
    return false;

  to->top_up(sum);
  Transaction t(from->get_id(), to->get_id(), sum);
  t.set_approved(
      send_request(from, to, Request(Request::TRANSFER, sender, t.get_id())));
  return send_transaction(t);
}

bool TransactionManager::make_transaction_request(size_t sender,
                                                  BankAccount *from,
                                                  BankAccount *to, size_t sum) {
  // check if accounts are not in freeze
  if (!from->is_available() || !to->is_available())
    return false;

  // can pay?
  if (!from->can_pay(sum))
    return false;

  Transaction t(from->get_id(), to->get_id(), sum);
  Request r(Request::TRANSFER_REQUEST, sender, t.get_id());
  r.set_approved(USER_DB->add_transaction(t));

  return IHistoryManager::send_request(r);
}

bool TransactionManager::make_transaction(size_t sender, BankAccount *acc,
                                          size_t dest, size_t sum) {
  std::unique_ptr<BankAccount> receiver(AccountManager::get_account(dest));
  if (!receiver)
    return false;

  receiver->top_up(sum);
  Transaction t(acc->get_id(), receiver->get_id(), sum);
  t.set_approved(send_request(acc, receiver.get(),
                              Request(Request::TRANSFER, sender, t.get_id())));
  return send_transaction(t);
}

bool TransactionManager::make_withdraw(size_t sender, BankAccount *acc,
                                       size_t sum) {
  Transaction t(acc->get_id(), sum, true);
  t.set_approved(
      send_request(acc, Request(Request::WITHDRAW, sender, t.get_id())));
  return send_transaction(t);
}

// Object

TransactionManager::TransactionManager(IUser *user, Mode mode)
    : IHistoryManager(user) {
  request_type =
      mode == Mode::CONDUCT ? Request::TRANSFER : Request::TRANSFER_REQUEST;

  connect(USER_DB, &DataBase::updated, this, &TransactionManager::update_vars);
  TransactionManager::update_vars();
}

std::vector<QTableWidgetItem *> TransactionManager::get_items() const {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {

    QString item = requests[i]->get_info();
    if (transactions[i])
      item += transactions[i]->get_info();
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

bool TransactionManager::mark(size_t item_index, bool verify) {
  std::unique_ptr<Transaction> current = std::move(transactions[item_index]);
  std::unique_ptr<Request> r = std::move(requests[item_index]);

  return request_type == Request::TRANSFER
             ? mark_transaction(current.get(), r.get(), verify)
             : mark_transaction_request(current.get(), r.get(), verify);
}

bool TransactionManager::mark_transaction(Transaction *t, Request *r,
                                          bool verify) {
  if (verify)
    return true;
  r->set_viewed(undo_transfer(user->get_id(), *t));
  if (r->is_viewed())
    qDebug() << "Request : " << USER_DB->update(*r);
  return r->is_viewed();
}

bool TransactionManager::mark_transaction_request(Transaction *t, Request *r,
                                                  bool verify) {
  if (!verify)
    return undo_transfer_request(user->get_id(), *r);

  std::unique_ptr<BankAccount> sender(
      AccountManager::get_account(t->get_sender()));
  std::unique_ptr<BankAccount> receiver(
      AccountManager::get_account(t->get_receiver()));
  if (!sender || !receiver)
    return false;

  // from other

  // check if accounts are not in freeze
  if (!sender->is_available() || !receiver->is_available())
    return false;

  // can pay?
  if (!sender->withdraw(t->get_amount()))
    return false;

  receiver->top_up(t->get_amount());
  t->set_approved(
      send_request(sender.get(), receiver.get(),
                   Request(Request::VERIFY, user->get_id(), t->get_id())));

  r->set_viewed(send_transaction(*t));
  if (r->is_viewed())
    qDebug() << "Request : " << USER_DB->update(*r);
  return r->is_viewed();
}

size_t TransactionManager::get_selected(size_t index) const {
  return transactions[index]->get_id();
}

void TransactionManager::update_vars() {
  transactions.clear();
  requests.clear();
  //  requests = USER_DB->get_transfer_requests(); it's for all types
  requests = USER_DB->get_requests(request_type, false);
  for (auto &r : requests) {
    transactions.push_back(USER_DB->get_transaction(r->get_object()));
  }
}
