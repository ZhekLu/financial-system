#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"
#include <cstdio>

class BankAccount : public ISystemObject {
public:
  BankAccount(size_t id, size_t owner_id, size_t bank_id, size_t balance,
              bool frozen);
  BankAccount(size_t owner_id, size_t bank_id);

  // methods
  void top_up(size_t value) { balance += value; }
  bool withdraw(size_t value);
  bool can_pay(size_t value) const { return balance >= value; }

  void set_frozen(bool value) { frozen = value; }
  void revert_frozen() { frozen = !frozen; }
  void set_balance(size_t value) { balance = value; } // TODO! remove

  // getters
  size_t get_balance() const { return balance; }
  size_t get_owner() const { return owner_id; }
  size_t get_bank() const { return bank_id; }
  bool is_frozen() const { return frozen; }

  // id, user_id, bank_id, balance, frozen
  QString get_values_query() override;
  QString get_update_query();
  QString get_info() const override;

private:
  size_t owner_id;
  size_t bank_id;
  size_t balance = 0;
  bool frozen;

  static inline IdGenerator id_creator{9, "accounts", "id"};
};

inline BankAccount::BankAccount(size_t id, size_t owner_id, size_t bank_id,
                                size_t balance, bool frozen)
    : ISystemObject(id), owner_id(owner_id), bank_id(bank_id), balance(balance),
      frozen(frozen) {}

inline BankAccount::BankAccount(size_t owner_id, size_t bank_id)
    : BankAccount(id_creator.GenerateId(), owner_id, bank_id, 0, 1) {}

inline bool BankAccount::withdraw(size_t value) {
  if (!can_pay(value))
    return false;
  balance -= value;
  return true;
}

inline QString BankAccount::get_values_query() {
  return QString("(%1, %2, %3, %4, %5)")
      .arg(QString::number(id), QString::number(owner_id),
           QString::number(bank_id), QString::number(balance),
           QString::number(frozen));
}

inline QString BankAccount::get_update_query() {
  return QString("balance = %1, frozen = %2")
      .arg(QString::number(balance), QString::number(frozen));
}

inline QString BankAccount::get_info() const {
  return QString("id: %1\n"
                 "balance: %2\n"
                 "frozen: %3\n")
      .arg(QString::number(id), QString::number(balance),
           frozen ? "True" : "False");
}

#endif // BANKACCOUNT_H
