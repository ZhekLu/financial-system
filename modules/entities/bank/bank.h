#ifndef BANK_H
#define BANK_H

#include "modules/database/idgenerator.h"
#include "modules/entities/entity.h"

class Bank : public Entity {
public:
  Bank(int percent, Type type, std::string name, size_t PAC, size_t BIC,
       std::string adress, size_t bank_id = 0)
      : Entity(type, std::move(name), PAC, BIC, std::move(adress), bank_id),
        percent(percent) {}
  // id, percents
  QString get_values_query() override {
    return QString("(%1, %2)")
        .arg(QString::number(id), QString::number(percent));
  }
  QString get_info() const override {
    return Entity::get_info() +
           QString("percent: %1\n").arg(QString::number(percent));
  }

  int get_percent() const { return percent; }

  int percent;
};

#endif // BANK_H
