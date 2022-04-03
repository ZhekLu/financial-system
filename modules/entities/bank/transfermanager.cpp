#include "transfermanager.h"

TransferManager::TransferManager() { TransferManager::update(); }

std::vector<QTableWidgetItem *> TransferManager::get_items() {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {
    QString item = requests[i]->get_info() + transactions[i]->get_info();
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

bool TransferManager::undo(size_t item_index) {}

void TransferManager::update() {
  transactions.clear();
  requests = USER_DB->get_transfer_requests();
  for (auto &r : requests) {
    transactions.push_back(USER_DB->get_transaction(r->object_id));
  }
}
