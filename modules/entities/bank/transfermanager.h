#ifndef TRANSFERMANAGER_H
#define TRANSFERMANAGER_H

#include "IHistoryManager.h"

class TransferManager : public IHistoryManager {
public:
  TransferManager(IUser *user);

  std::vector<QTableWidgetItem *> get_items() override;
  bool undo(size_t item_index) override;

private slots:
  void update() override;

private:
  std::vector<std::unique_ptr<Transaction>> transactions;
  std::vector<std::unique_ptr<Request>> requests;
};

#endif // TRANSFERMANAGER_H
