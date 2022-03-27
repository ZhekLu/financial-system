#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "modules/entities/ISystemObject.h"
#include <cstdio>

class BankAccount : public ISystemObject {
public:
  BankAccount(size_t id, size_t owner_id, size_t bank_id, size_t balance,
              bool frozen)
      : ISystemObject(id), owner_id(owner_id), bank_id(bank_id),
        balance(balance), is_frozen(frozen) {}

  // methods
  void top_up(size_t value) { balance += value; }
  void withdraw(size_t value) { balance -= value; }
  bool can_pay(size_t value) const { return balance >= value; }

  // getters
  size_t get_balance() const { return balance; }
  size_t get_id() const { return id; }
  size_t get_owner_id() const { return owner_id; }
  size_t get_bank_id() const { return bank_id; }
  bool get_frozen() const { return is_frozen; }
  QString get_values_query() override { return {}; }
  QString get_info() const override {
    return QString("id: %1\n"
                   "balance: %2\n"
                   "frozen: %3\n")
        .arg(QString::number(id), QString::number(balance),
             is_frozen ? "True" : "False");
  }

  size_t owner_id;
  size_t bank_id;
  size_t balance = 0;
  bool is_frozen;
};

#endif // BANKACCOUNT_H
