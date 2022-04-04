#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "IUser.h"

class Individual : public IUser {
public:
  Individual(std::string full_name, std::string pasport_number,
             std::string pasport_id, std::string phone_number,
             std::string email, size_t id = 0);

  ~Individual() = default;

  // methods
  QString get_info() const override;
  QString get_values_query() override;

private:
  std::string name;
  std::string pass_number;
  std::string pass_id;
  std::string phone_number;
  std::string email;
};

inline Individual::Individual(std::string full_name, std::string pasport_number,
                              std::string pasport_id, std::string phone_number,
                              std::string email, size_t id)
    : IUser(id, LoginMode::INDIVIDUAL), name(std::move(full_name)),
      pass_number(std::move(pasport_number)), pass_id(std::move(pasport_id)),
      phone_number(std::move(phone_number)), email(std::move(email)) {}

inline QString Individual::get_info() const {
  return QString("Full name: %1\n"
                 "Passport number: %2\n"
                 "Passport id: %3\n"
                 "Phone number: %4\n"
                 "email: %5\n")
      .arg(QString::fromStdString(name), QString::fromStdString(pass_number),
           QString::fromStdString(pass_id),
           QString::fromStdString(phone_number), QString::fromStdString(email));
}

inline QString Individual::get_values_query() { return {}; }

#endif // INDIVIDUAL_H
