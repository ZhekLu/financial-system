#include "accountaddmanager.h"

// Static

bool AccountAddManager::deposit_request(size_t initiator_id, size_t bank_id,
                                        size_t account_id,
                                        size_t period_in_month,
                                        size_t percent) {
  auto account = USER_DB->get_account(account_id);
  if (!account || !account->is_available())
    return false;

  AccountAdd deposit(initiator_id, bank_id, account_id, QDate::currentDate(),
                     period_in_month, account->get_balance(), percent);
  Request r(Request::DEPOSIT, initiator_id, deposit.get_id());

  return send_add(deposit, r);
}

bool AccountAddManager::salary_request(size_t initiator_id, size_t bank_id,
                                       size_t account_id,
                                       size_t period_in_month,
                                       size_t salary_amount) {
  auto account = USER_DB->get_account(account_id);
  if (!account || !account->is_available())
    return false;

  AccountAdd deposit(initiator_id, bank_id, account_id, QDate::currentDate(),
                     period_in_month, salary_amount);
  Request r(Request::SALARY, initiator_id, deposit.get_id());

  return send_add(deposit, r);
}

// Object

AccountAddManager::AccountAddManager(IUser *user, AccountAdd::Type add_type)
    : IHistoryManager(user, ItemsType::SYSTEM), add_type(add_type) {}

std::vector<QTableWidgetItem *> AccountAddManager::get_items() const {}

bool AccountAddManager::mark(size_t item_index, bool verify) {}

size_t AccountAddManager::get_selected(size_t index) const {}

void AccountAddManager::update_vars() {}

bool AccountAddManager::send_add(AccountAdd &a) { return USER_DB->add_add(a); }

bool AccountAddManager::send_add(AccountAdd &a, Request &r) {
  r.set_approved(send_add(a));
  return IHistoryManager::send_request(r);
}
