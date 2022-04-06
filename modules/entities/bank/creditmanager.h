#ifndef CREDITMANAGER_H
#define CREDITMANAGER_H

#include "IHistoryManager.h"
#include "credit.h"
#include "installment.h"
#include "modules/database/userdb.h"
#include "modules/entities/IUser.h"
#include "modules/entities/bank/bank.h"
#include "request.h"

class LoanManager : public IHistoryManager {
public:
  enum LoanType { CREDIT, INSTALLMENT };

  static bool credit_request(IUser *user, Bank *bank, size_t amount,
                             size_t period_in_months);
  static bool credit_request(IUser *user, Credit &c);
  static bool loan_request(IUser *user, Loan &l, bool is_credit);
  static bool loan_request(IUser *user, Loan &l, LoanType type);

  LoanManager(IUser *user, bool viewed);
  std::vector<QTableWidgetItem *> get_items() override;
  bool mark(size_t item_index, bool verify) override;

private slots:
  void update_vars() override;

private:
  static bool send_request(Credit *c, Request &r);
  static bool send_credit(Credit &c);
  static bool send_loan(Loan &l);

  bool viewed_mode;

  std::vector<std::unique_ptr<Credit>> credits;
  std::vector<std::unique_ptr<Request>> requests;
};

#endif // CREDITMANAGER_H
