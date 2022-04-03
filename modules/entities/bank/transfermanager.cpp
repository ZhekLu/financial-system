#include "transfermanager.h"

TransferManager::TransferManager(IUser *user) : IHistoryManager(user) {
  TransferManager::update();
}

std::vector<QTableWidgetItem *> TransferManager::get_items() {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {

    QString item = requests[i]->get_info();
    if (transactions[i])
      item += transactions[i]->get_info();
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

bool TransferManager::undo(size_t item_index) {
  Transaction *current = transactions[item_index].get();
  return AccountManager::undo_transfer_request(user->get_id(), *current);
}

void TransferManager::update() {
  transactions.clear();
  requests = USER_DB->get_transfer_requests();
  for (auto &r : requests) {
    transactions.push_back(USER_DB->get_transaction(r->object_id));
  }
}
