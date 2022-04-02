#ifndef CREDITMANAGER_H
#define CREDITMANAGER_H

#include "credit.h"
#include "modules/database/userdb.h"
#include "modules/entities/IUser.h"
#include "modules/entities/bank/bank.h"
#include "request.h"

class CreditManager {
public:
  static bool credit_request(IUser *user, Bank *bank, size_t amount,
                             size_t period_in_months);
  static bool credit_request(IUser *user, Credit &c);

private:
  static bool send_request(Request &r);
  static bool send_credit(Credit &c);
};

#endif // CREDITMANAGER_H
