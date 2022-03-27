#ifndef ENTITY_H
#define ENTITY_H

#include "IUser.h"

class Entity : public IUser {
public:
  enum Type { IE, LLC, CJSC, OJSC };

  Entity(size_t id, Type type, std::string name, size_t PAC, size_t BIC,
         std::string adress, size_t bank_id)
      : IUser(id, std::move(name)), type(type), PAC(PAC), BIC(BIC),
        adress(std::move(adress)), bank_bic(bank_id) {}

  ~Entity() = default;

  // methods
  QString get_info() const override { return {}; }
  QString get_values_query() override { return {}; }

  Type type;
  std::string name;
  size_t PAC; // YNP
  size_t BIC;
  std::string adress;
  size_t bank_bic;
};

#endif // ENTITY_H
