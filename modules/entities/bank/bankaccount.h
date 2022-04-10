#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"
#include <cstdio>

class BankAccount : public ISystemObject {
public:
  BankAccount(size_t id, size_t owner_id, size_t bank_id, size_t balance = 0,
              bool frozen = false, bool blocked = false,
              bool is_available = false);
  BankAccount(size_t owner_id, size_t bank_id);

  // methods
  void top_up(size_t value) { balance += value; }
  bool withdraw(size_t value);
  bool can_pay(size_t value) const { return balance >= value; }
  void revert_frozen() { frozen = !frozen; }
  void set_available(bool value) { available = value; }

  // getters
  size_t get_balance() const { return balance; }
  size_t get_owner() const { return owner_id; }
  size_t get_bank() const { return bank_id; }
  bool is_frozen() const { return frozen; }
  bool is_blocked() const { return blocked; }
  bool is_available() const { return !frozen && !blocked && available; }

  // id, user_id, bank_id, balance, frozen, blocked, available
  QString get_values_query() override;
  QString get_update_query();
  QString get_info() const override;

private:
  size_t owner_id;
  size_t bank_id;
  size_t balance = 0;
  bool frozen;
  bool blocked;
  bool available;

  static inline IdGenerator id_creator{9, "accounts", "id"};
};

inline BankAccount::BankAccount(size_t id, size_t owner_id, size_t bank_id,
                                size_t balance, bool frozen, bool blocked,
                                bool is_available)
    : ISystemObject(id), owner_id(owner_id), bank_id(bank_id), balance(balance),
      frozen(frozen), blocked(blocked), available(is_available) {}

inline BankAccount::BankAccount(size_t owner_id, size_t bank_id)
    : BankAccount(id_creator.GenerateId(), owner_id, bank_id) {}

inline bool BankAccount::withdraw(size_t value) {
  if (!can_pay(value) || !is_available())
    return false;
  balance -= value;
  return true;
}

inline QString BankAccount::get_values_query() {
  // id, user_id, bank_id, balance, frozen, blocked, available
  return QString("(%1, %2, %3, %4, %5, %6, %7)")
      .arg(QString::number(id), QString::number(owner_id),
           QString::number(bank_id), QString::number(balance),
           QString::number(frozen), QString::number(blocked),
           QString::number(available));
}

inline QString BankAccount::get_update_query() {
  return QString("balance = %1, frozen = %2, blocked = %3, available = %4")
      .arg(QString::number(balance), QString::number(frozen),
           QString::number(blocked), QString::number(available));
}

inline QString BankAccount::get_info() const {
  return QString("id: %1\n"
                 "balance: %2\n"
                 "frozen: %3\n"
                 "blocked: %4\n")
      .arg(QString::number(id), QString::number(balance),
           frozen ? "True" : "False", blocked ? "True" : "False");
}

#endif // BANKACCOUNT_H
