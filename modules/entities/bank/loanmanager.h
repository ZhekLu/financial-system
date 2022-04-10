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

  static bool loan_request(IUser *user, Loan &l, LoanType type);

  // non static
  LoanManager(LoanType loan_type, IUser *user, ItemsType items_type);

  bool mark(size_t item_index, bool verify) override;
  size_t get_selected(size_t index) const override;
  std::vector<QTableWidgetItem *> get_items() const override;

private slots:
  void update_vars() override;

private:
  // static methods
  static bool send_request(Credit *c, Request &r);
  static bool send_credit(Credit &c);
  static bool send_loan(Loan &l);

  // fields
  LoanType loan_type;
  Request::Type request_type;
  std::vector<std::unique_ptr<Loan>> credits;
  std::vector<std::unique_ptr<Request>> requests;

  void update_requests();
};

#endif // CREDITMANAGER_H
