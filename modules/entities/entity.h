#ifndef ENTITY_H
#define ENTITY_H

#include "IUser.h"

class Entity : public IUser {
public:
  enum Type { IE, LLC, CJSC, OJSC };

  Entity(Type type, std::string name, size_t PAC, size_t BIC,
         std::string adress, size_t bank_id)
      : IUser(BIC, LoginMode::ENTITY, std::move(name)), type(type), PAC(PAC),
        BIC(BIC), adress(std::move(adress)), bank_bic(bank_id) {}

  ~Entity() = default;

  // methods
  QString get_info() const override {
    return QString("Type: %1\n"
                   "Legal name: %2\n"
                   "PAC: %3\n"
                   "BIC: %4\n"
                   "Legal adress: %5\n"
                   "Ur bank credits: %6\n")
        .arg(QString::number(type), QString::fromStdString(name),
             QString::number(PAC), QString::number(BIC),
             QString::fromStdString(adress), QString::number(bank_bic));
  }
  QString get_values_query() override { return {}; }

  Type type;
  size_t PAC; // YNP
  size_t BIC;
  std::string adress;
  size_t bank_bic;
};

#endif // ENTITY_H
