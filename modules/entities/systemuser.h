#ifndef SYSTEMUSER_H
#define SYSTEMUSER_H

#include "modules/modes.h"
#include <string>

struct SystemUser {
  SystemUser(std::string login, std::string password, LoginMode lm,
             size_t id = -1)
      : login(login), password(password), mode(lm), user_id(id) {}
  // fields
  std::string login;
  std::string password;
  LoginMode mode;
  size_t user_id;
};

#endif // SYSTEMUSER_H
