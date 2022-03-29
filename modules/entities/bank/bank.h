#ifndef BANK_H
#define BANK_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

class Bank : public ISystemObject {
public:
  Bank(size_t id, int percent) : ISystemObject(id), percent(percent) {}

  // id, percents
  QString get_values_query() override {
    return QString("(%1, %2)")
        .arg(QString::number(id), QString::number(percent));
  }
  QString get_info() const override {
    return QString("bic: %1\n"
                   "percent: %2\n")
        .arg(QString::number(id), QString::number(percent));
  }

  int percent;
};

#endif // BANK_H
