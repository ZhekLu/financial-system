#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include "IHistoryManager.h"
#include "bankaccount.h"
#include "modules/database/userdb.h"
#include "request.h"

class TransactionManager : public IHistoryManager {
  Q_OBJECT
public:
  enum Mode { CONDUCT, REQUEST };

  static bool withdraw_request(size_t sender_id, size_t account_id, size_t sum);
  static bool transfer_request(size_t sender_id, size_t account_id,
                               size_t receiver_id, size_t sum);

  static bool entity_transfer_request(size_t sender_id, size_t account_id,
                                      size_t receiver_id, size_t sum);

  // Manager
  TransactionManager(IUser *user, Mode mode = Mode::CONDUCT);

  std::vector<QTableWidgetItem *> get_items() const override;
  bool mark(size_t item_index, bool verify = false) override;
  size_t get_selected(size_t index) const override;

private slots:
  void update_vars() override;

private:
  Request::Type request_type;

  std::vector<std::unique_ptr<Transaction>> transactions;
  std::vector<std::unique_ptr<Request>> requests;

  static bool send_request(BankAccount *acc, Request);
  static bool send_request(BankAccount *acc, BankAccount *sec, Request r);

  // Make

  static bool send_transaction(Transaction &);
  static bool make_transaction(size_t sender, BankAccount *from,
                               BankAccount *to, size_t sum);
  static bool make_transaction_request(size_t sender, BankAccount *from,
                                       BankAccount *to, size_t sum);
  static bool make_transaction(size_t sender, BankAccount *acc, size_t dest,
                               size_t sum);
  static bool make_withdraw(size_t sender, BankAccount *acc, size_t sum);

  // Undo

  static bool undo_transfer(size_t initiator, Transaction &t);
  static bool undo_transfer_request(size_t initiator, Request &original);
  static bool make_undo_transaction(size_t initiator, BankAccount *sen,
                                    BankAccount *rec, Transaction &original);
  static bool make_undo_withdraw(size_t initiator, BankAccount *acc,
                                 Transaction &original);

  // Verify

  static bool verify_transfer_request(size_t initiator, Transaction &t);

  // Mark

  bool mark_transaction(Transaction *t, Request *r, bool verify);
  bool mark_transaction_request(Transaction *t, Request *r, bool verify);
};

#endif // TRANSACTIONMANAGER_H
