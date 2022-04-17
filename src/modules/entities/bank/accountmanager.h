#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "IManager.h"

class AccountManager : public IManager {
public:
  static std::unique_ptr<BankAccount> get_account(size_t id);
  static bool update(BankAccount *acc);

  // Manager
  AccountManager(IUser *user);
  std::vector<QTableWidgetItem *> get_items() const override;
  size_t get_selected(size_t index) const override;
  BankAccount *get_selected_account(size_t index) const;

private slots:
  void update_vars() override;

private:
  // variables
  std::vector<std::unique_ptr<BankAccount>> accounts;
  std::unordered_map<size_t, std::unique_ptr<Bank>> banks;

  // methods
  static void update_account(BankAccount *acc);
  static void update_account_adds(size_t id);
  QTableWidgetItem *get_item(BankAccount *acc, QString bank) const;
};

#endif // ACCOUNTMANAGER_H
