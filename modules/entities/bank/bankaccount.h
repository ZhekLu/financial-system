#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"
#include <cstdio>

class BankAccount : public ISystemObject {
public:
  BankAccount(size_t id, size_t owner_id, size_t bank_id, size_t balance,
              bool frozen)
      : ISystemObject(id), owner_id(owner_id), bank_id(bank_id),
        balance(balance), is_frozen(frozen) {}
  BankAccount(size_t owner_id, size_t bank_id)
      : BankAccount(id_creator.GenerateId(), owner_id, bank_id, 0, 1) {}

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

  // id, user_id, bank_id, balance, frozen
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4, %5)")
        .arg(QString::number(id), QString::number(owner_id),
             QString::number(bank_id), QString::number(balance),
             QString::number(is_frozen));
  }
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

private:
  static inline IdGenerator id_creator{9, "accounts", "id"};
};

#endif // BANKACCOUNT_H
