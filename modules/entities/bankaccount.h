#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include <cstdio>

class BankAccount {
public:
  BankAccount(size_t id, size_t owner_id, size_t bank_id, size_t balance)
      : id(id), owner_id(owner_id), bank_id(bank_id), balance(balance) {}

  // methods
  void top_up(size_t value) { balance += value; }
  void withdraw(size_t value) { balance -= value; }
  bool can_pay(size_t value) const { return balance >= value; }

  // getters
  size_t get_balance() const { return balance; }
  size_t get_id() const { return id; }
  size_t get_owner_id() const { return owner_id; }
  size_t get_bank_id() const { return bank_id; }

private:
  size_t id;
  size_t owner_id;
  size_t bank_id;
  size_t balance = 0;
};

#endif // BANKACCOUNT_H
