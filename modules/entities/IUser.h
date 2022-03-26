#ifndef IUSER_H
#define IUSER_H

#include "bankaccount.h"

#include <QString>
#include <string>
#include <unordered_map>

class IUser {
public:
  IUser(size_t id, std::string full_name)
      : id(id), name(std::move(full_name)) {}
  virtual ~IUser() = default;

  // methods
  virtual QString get_info() const = 0;
  std::string get_name() const { return name; }
  size_t get_id() const { return id; }
  const auto &get_accounts() const { return accounts; }

protected:
  size_t id;
  std::string name;
  std::unordered_map<size_t, BankAccount *> accounts;
};

#endif // IUSER_H
