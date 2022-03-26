#ifndef USER_H
#define USER_H

#include <string>

struct User {
  User(std::string name, std::string pasport_number, std::string pasport_id,
       std::string phone_number, std::string email, size_t id = -1)
      : name(name), pass_number(pasport_number), pass_id(pasport_id),
        phone_number(phone_number), email(email), id(id) {}

  std::string name;
  std::string pass_number;
  std::string pass_id;
  std::string phone_number;
  std::string email;
  size_t id;
};

#endif // USER_H
