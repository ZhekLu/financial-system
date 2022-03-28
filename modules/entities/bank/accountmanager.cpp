#include "accountmanager.h"

bool AccountManager::freeze_request(BankAccount *acc) {
  if (!update(acc))
    return false;
  bool old = acc->is_frozen;
  acc->is_frozen = !old;
  if (send_request(acc, Request(Request::FREEZE, acc->id)))
    return true;
  acc->is_frozen = old;
  return false;
}

bool AccountManager::withdraw_request(BankAccount *acc, size_t sum) {

  if (!update(acc))
    return false;
  size_t old = acc->balance;
  acc->balance = old - sum;
  if (make_withdraw(acc, sum))
    return true;
  acc->balance = old;
  return false;
}

bool AccountManager::transfer_request(BankAccount *acc, size_t destination,
                                      size_t sum) {
  if (!update(acc))
    return false;
  size_t old = acc->balance;
  acc->balance = old - sum;
  if (make_transaction(acc, destination, sum))
    return true;
  acc->balance = old;
  return false;
}

bool AccountManager::add_account_request(BankAccount *acc) {
  Request r(Request::LOGIN_ACCOUNT, acc->id);
  r.is_approved = USER_DB->add_account(acc);
  return send_request(r);
}

bool AccountManager::update(BankAccount *acc) {
  if (check_valid(acc))
    return true;
  acc->balance = USER_DB->get_account_balance(acc->id);
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

  receiver->balance += sum;
  Transaction t(acc->id, receiver->id, sum);
  t.is_approved =
      send_request(acc, receiver.get(), Request(Request::TRANSFER, acc->id));
  return send_transaction(t);
}

bool AccountManager::make_withdraw(BankAccount *acc, size_t sum) {
  Transaction t(acc->id, sum);
  t.is_approved = send_request(acc, Request(Request::WITHDRAW, acc->id));
  return send_transaction(t);
}
