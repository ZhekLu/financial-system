#include "transactionmanager.h"

// Static

bool TransactionManager::freeze_request(size_t sender_id, size_t account_id,
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

bool TransactionManager::withdraw_request(size_t sender_id, size_t account_id,
                                          size_t sum) {
  std::unique_ptr<BankAccount> acc(USER_DB->get_account(account_id));
  if (!acc || !acc->withdraw(sum))
    return false;
  return make_withdraw(sender_id, acc.get(), sum);
}

bool TransactionManager::transfer_request(size_t sender_id, size_t account_id,
                                          size_t receiver_account_id,
                                          size_t sum) {
  std::unique_ptr<BankAccount> sender(USER_DB->get_account(account_id));
  std::unique_ptr<BankAccount> receiver(
      USER_DB->get_account(receiver_account_id));
  if (!sender || !receiver)
    return false;
  return make_transaction(sender_id, sender.get(), receiver.get(), sum);
}

bool TransactionManager::undo_transfer_request(size_t initiator,
                                               Transaction &t) {
  std::unique_ptr<BankAccount> sender(USER_DB->get_account(t.get_sender()));
  std::unique_ptr<BankAccount> receiver(USER_DB->get_account(t.get_receiver()));
  if (!sender || (t.get_type() == Transaction::TRANSFER &&
                  (!receiver || !receiver->can_pay(t.get_amount()))))
    return false;

  return t.get_type() == Transaction::TRANSFER
             ? make_undo_transaction(initiator, sender.get(), receiver.get(), t)
             : make_undo_withdraw(initiator, sender.get(), t);
}

bool TransactionManager::make_undo_transaction(size_t initiator,
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

bool TransactionManager::make_undo_withdraw(size_t initiator, BankAccount *acc,
                                            Transaction &original) {
  acc->top_up(original.get_amount());
  Transaction ut(acc->get_id(), original.get_amount(), false);
  ut.set_approved(
      send_request(acc, Request(Request::UNDO, initiator, ut.get_id())));
  return send_transaction(ut);
}

bool TransactionManager::send_request(BankAccount *acc, Request r) {
  r.set_approved(USER_DB->update(*acc));
  return IHistoryManager::send_request(r);
}

bool TransactionManager::send_request(BankAccount *acc, BankAccount *sec,
                                      Request r) {
  r.set_approved(USER_DB->update(*acc));
  if (r.is_approved())
    r.set_approved(USER_DB->update(*sec));
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

bool TransactionManager::make_transaction(size_t sender, BankAccount *acc,
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

bool TransactionManager::make_withdraw(size_t sender, BankAccount *acc,
                                       size_t sum) {
  Transaction t(acc->get_id(), sum, true);
  t.set_approved(
      send_request(acc, Request(Request::WITHDRAW, sender, t.get_id())));
  return send_transaction(t);
}

// Object

TransactionManager::TransactionManager(IUser *user) : IHistoryManager(user) {
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
  if (verify)
    return true;
  std::unique_ptr<Transaction> current = std::move(transactions[item_index]);
  std::unique_ptr<Request> r = std::move(requests[item_index]);

  r->set_viewed(undo_transfer_request(user->get_id(), *current));
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
  requests = USER_DB->get_transfer_requests();
  for (auto &r : requests) {
    transactions.push_back(USER_DB->get_transaction(r->get_object()));
  }
}
