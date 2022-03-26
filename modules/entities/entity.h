#ifndef ENTITY_H
#define ENTITY_H

#include "IUser.h"

class Entity : public IUser {
public:
  enum Type { IE, LLC, CJSC, OJSC };

  Entity(size_t id, Type type, std::string name, std::string PAC,
         std::string BIC, std::string adress)
      : IUser(id, std::move(name)), type(type), PAC(std::move(PAC)),
        BIC(std::move(BIC)), adress(std::move(adress)) {}

  ~Entity() = default;

  // methods
  QString get_info() override { return {}; }

private:
  Type type;
  std::string name;
  std::string PAC; // YNP
  std::string BIC;
  std::string adress;
};

#endif // ENTITY_H
