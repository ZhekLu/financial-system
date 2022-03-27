#ifndef BALANCEMANAGER_H
#define BALANCEMANAGER_H

#include "bankaccount.h"

class BalanceManager {
public:
  static bool top_up_balance(size_t account_id);
  static bool withdraw_balance(size_t account_id);
};

#endif // BALANCEMANAGER_H
