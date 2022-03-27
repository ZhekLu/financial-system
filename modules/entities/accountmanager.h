#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include "bankaccount.h"
#include "modules/database/userdb.h"
#include "request.h"

class AccountManager {
public:
  static bool freeze_request(BankAccount *acc);
  static bool withdraw_request(BankAccount *acc, size_t sum);
  static bool transfer_request(BankAccount *acc, size_t destination,
                               size_t sum);

private:
  static bool update(BankAccount *acc);
  static bool check_valid(BankAccount *acc);
  static bool send_request(BankAccount *acc, Request);
};

#endif // ACCOUNTMANAGER_H
