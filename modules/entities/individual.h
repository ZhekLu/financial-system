#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "IUser.h"

class Individual : public IUser {
public:
  Individual(std::string full_name, std::string pasport_number,
             std::string pasport_id, std::string phone_number,
             std::string email, size_t id = -1)
      : IUser(id, std::move(full_name)), pass_number(pasport_number),
        pass_id(pasport_id), phone_number(phone_number), email(email) {}

  QString get_info() override { return {}; }

private:
  std::string pass_number;
  std::string pass_id;
  std::string phone_number;
  std::string email;
};

#endif // INDIVIDUAL_H
