#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "IHistoryManager.h"
#include "bankaccount.h"
#include "modules/database/userdb.h"
#include "request.h"

class AccountManager : public IHistoryManager {
public:
  static bool freeze_request(BankAccount *acc);
  static bool withdraw_request(BankAccount *acc, size_t sum);
  static bool transfer_request(BankAccount *acc, size_t destination,
                               size_t sum);
  static bool undo_transfer_request(size_t initiator, Transaction &t);
  static bool add_account_request(BankAccount *acc);

  // Manager
  AccountManager(IUser *user);

  std::vector<QTableWidgetItem *> get_items() override;
  bool undo(size_t item_index) override;

private slots:
  void update() override;

private:
  std::vector<std::unique_ptr<Transaction>> transactions;
  std::vector<std::unique_ptr<Request>> requests;

  static bool update(BankAccount *acc);
  static bool check_valid(BankAccount *acc);
  static bool send_request(Request &);
  static bool send_request(BankAccount *acc, Request);
  static bool send_request(BankAccount *acc, BankAccount *sec, Request r);

  static bool send_transaction(Transaction &);
  static bool make_transaction(BankAccount *acc, size_t dest, size_t sum);
  static bool make_withdraw(BankAccount *acc, size_t sum);
};

#endif // ACCOUNTMANAGER_H
