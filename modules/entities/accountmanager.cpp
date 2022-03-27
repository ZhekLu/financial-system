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

bool AccountManager::update(BankAccount *acc) {
  if (check_valid(acc))
    return true;
  acc->balance = USER_DB->get_account_balance(acc->id);
  return check_valid(acc);
}

bool AccountManager::check_valid(BankAccount *acc) {
  return USER_DB->contains(*acc);
}

bool AccountManager::send_request(BankAccount *acc, Request r) {
  r.is_approved = USER_DB->update(*acc);
  USER_DB->add_request(r);
  return r.is_approved;
}
