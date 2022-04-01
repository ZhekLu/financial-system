#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "IUser.h"

class Individual : public IUser {
public:
  Individual(std::string full_name, std::string pasport_number,
             std::string pasport_id, std::string phone_number,
             std::string email, size_t id = -1)
      : IUser(id, LoginMode::INDIVIDUAL, std::move(full_name)),
        pass_number(std::move(pasport_number)), pass_id(std::move(pasport_id)),
        phone_number(std::move(phone_number)), email(std::move(email)) {}

  ~Individual() = default;

  // methods
  QString get_info() const override {
    return QString("Full name: %1\n"
                   "Passport number: %2\n"
                   "Passport id: %3\n"
                   "Phone number: %4\n"
                   "email: %5\n")
        .arg(QString::fromStdString(name), QString::fromStdString(pass_number),
             QString::fromStdString(pass_id),
             QString::fromStdString(phone_number),
             QString::fromStdString(email));
  }

  QString get_values_query() override { return {}; }

private:
  std::string pass_number;
  std::string pass_id;
  std::string phone_number;
  std::string email;
};

#endif // INDIVIDUAL_H
