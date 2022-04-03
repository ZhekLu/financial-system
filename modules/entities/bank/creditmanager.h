#ifndef CREDITMANAGER_H
#define CREDITMANAGER_H

#include "IHistoryManager.h"
#include "credit.h"
#include "modules/database/userdb.h"
#include "modules/entities/IUser.h"
#include "modules/entities/bank/bank.h"
#include "request.h"

class CreditManager : public IHistoryManager {
public:
  static bool credit_request(IUser *user, Bank *bank, size_t amount,
                             size_t period_in_months);
  static bool credit_request(IUser *user, Credit &c);

  CreditManager(IUser *user);
  std::vector<QTableWidgetItem *> get_items() override;
  bool undo(size_t item_index) override;

private slots:
  void update() override;

private:
  static bool send_request(Request &r);
  static bool send_request(Credit *c, Request &r);

  static bool send_credit(Credit &c);

  std::vector<std::unique_ptr<Credit>> credits;
  std::vector<std::unique_ptr<Request>> requests;
};

#endif // CREDITMANAGER_H
