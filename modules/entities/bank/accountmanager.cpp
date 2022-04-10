#include "accountmanager.h"

// Static

bool AccountManager::add_account_request(size_t sender_id, BankAccount *acc) {
  Request r(Request::LOGIN_ACCOUNT, sender_id, acc->get_id());
  r.set_approved(USER_DB->add_account(acc));
  return IHistoryManager::send_request(r);
}

// Object

AccountManager::AccountManager(IUser *user) : IHistoryManager(user) {
  connect(USER_DB, &DataBase::updated, this, &AccountManager::update_vars);
  AccountManager::update_vars();
}

std::vector<QTableWidgetItem *> AccountManager::get_items() const {}

bool AccountManager::mark(size_t item_index, bool verify) {}

size_t AccountManager::get_selected(size_t index) const {}

void AccountManager::update_vars() {}
