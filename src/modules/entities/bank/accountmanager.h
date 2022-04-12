#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "IHistoryManager.h"

class AccountManager : public IHistoryManager {
  Q_OBJECT
public:
  static bool freeze_request(size_t sender_id, size_t account_id, bool freeze);
  static bool block_request(size_t sender_id, size_t account_id, bool freeze);
  static bool add_account_request(size_t sender_id, BankAccount *acc);
  static bool add_account_request(size_t sender_id, size_t bank_id);

  // Manager

  enum Mode { LOGIN, BLOCK };

  AccountManager(IUser *user, Mode mode,
                 ItemsType items_type = ItemsType::SYSTEM);

  std::vector<QTableWidgetItem *> get_items() const override;
  bool mark(size_t item_index, bool verify = false) override;
  size_t get_selected(size_t index) const override;

private slots:
  void update_vars() override;

private:
  Mode mode;

  // values
  std::vector<std::unique_ptr<BankAccount>> accounts;
  std::vector<std::unique_ptr<Request>> requests;

  bool approve_login(BankAccount *acc);
};

#endif // ACCOUNTMANAGER_H
