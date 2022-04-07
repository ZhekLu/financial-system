#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "IHistoryManager.h"
#include "bankaccount.h"
#include "modules/database/userdb.h"
#include "request.h"

class AccountManager : public IHistoryManager {
  Q_OBJECT
public:
  static bool freeze_request(size_t sender_id, size_t account_id, bool freeze);
  static bool withdraw_request(size_t sender_id, size_t account_id, size_t sum);
  static bool transfer_request(size_t sender_id, size_t account_id,
                               size_t receiver_id, size_t sum);
  static bool add_account_request(size_t sender_id, BankAccount *acc);

  // Manager
  AccountManager(IUser *user);

  std::vector<QTableWidgetItem *> get_items() const override;
  bool mark(size_t item_index, bool verify = false) override;
  size_t get_selected(size_t index) const override;

private slots:
  void update_vars() override;

private:
  std::vector<std::unique_ptr<Transaction>> transactions;
  std::vector<std::unique_ptr<Request>> requests;

  static bool send_request(BankAccount *acc, Request);
  static bool send_request(BankAccount *acc, BankAccount *sec, Request r);

  static bool send_transaction(Transaction &);
  static bool make_transaction(size_t sender, BankAccount *acc, size_t dest,
                               size_t sum);
  static bool make_withdraw(size_t sender, BankAccount *acc, size_t sum);

  static bool undo_transfer_request(size_t initiator, Transaction &t);
  static bool make_undo_transaction(size_t initiator, BankAccount *sen,
                                    BankAccount *rec, Transaction &original);
  static bool make_undo_withdraw(size_t initiator, BankAccount *acc,
                                 Transaction &original);
};

#endif // ACCOUNTMANAGER_H
