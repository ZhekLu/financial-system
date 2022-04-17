#ifndef ENTITY_H
#define ENTITY_H

#include "IUser.h"

class Entity : public IUser {
public:
  enum Type { IE, LLC, CJSC, OJSC };

  static inline std::vector<QString> type_string{"IE", "LLC", "CJSC", "OJSC"};

  Entity(Type type, std::string name, size_t PAC, size_t BIC,
         std::string adress, size_t bank_id);

  ~Entity() = default;

  // methods
  QString get_info() const override;
  //(id, name, type, PAC, BIC, adress, bank_bic)
  QString get_values_query() override;

  QString get_name() const { return QString::fromStdString(name); }
  size_t get_bank() const { return bank_bic; }

private:
  std::string name;
  Type type;
  size_t PAC; // YNP
  size_t BIC;
  std::string adress;
  size_t bank_bic;
};

inline Entity::Entity(Type type, std::string name, size_t PAC, size_t BIC,
                      std::string adress, size_t bank_id)
    : IUser(BIC, LoginMode::ENTITY), name(std::move(name)), type(type),
      PAC(PAC), BIC(BIC), adress(std::move(adress)), bank_bic(bank_id) {}

inline QString Entity::get_info() const {
  return QString("Type: %1\n"
                 "Legal name: %2\n"
                 "PAC: %3\n"
                 "BIC: %4\n"
                 "Legal adress: %5\n"
                 "Ur bank credits: %6\n")
      .arg(type_string[type], QString::fromStdString(name),
           QString::number(PAC), QString::number(BIC),
           QString::fromStdString(adress), QString::number(bank_bic));
}

inline QString Entity::get_values_query() {
  return QString("(%1, '%2', %3, %4, %5, %6, %7)")
      .arg(QString::number(id), QString::fromStdString(name),
           QString::number(type), QString::number(PAC), QString::number(BIC),
           QString::fromStdString(adress), QString::number(bank_bic));
}

#endif // ENTITY_H
