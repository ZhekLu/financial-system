#include "accountaddmanager.h"

bool AccountAddManager::deposit_request(size_t initiator_id, size_t bank_id,
                                        size_t account_id,
                                        size_t period_in_month,
                                        size_t percent) {
  return true;
}

bool AccountAddManager::salary_request(size_t initiator_id, size_t bank_id,
                                       size_t account_id,
                                       size_t period_in_month,
                                       size_t salary_amount) {}

AccountAddManager::AccountAddManager(IUser *user, AccountAdd::Type add_type)
    : IHistoryManager(user, ItemsType::SYSTEM), add_type(add_type) {}

std::vector<QTableWidgetItem *> AccountAddManager::get_items() const {}

bool AccountAddManager::mark(size_t item_index, bool verify) {}

size_t AccountAddManager::get_selected(size_t index) const {}

void AccountAddManager::update_vars() {}
