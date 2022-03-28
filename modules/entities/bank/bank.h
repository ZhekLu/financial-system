#ifndef BANK_H
#define BANK_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

class Bank : public ISystemObject {
public:
  Bank(size_t id, size_t account_id, size_t percent)
      : ISystemObject(id), account(account_id), percent(percent) {}

  // id, account, percents
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4, %5)")
        .arg(QString::number(id), QString::number(account),
             QString::number(percent));
  }
  QString get_info() const override {
    return QString("bic: %1\n"
                   "percent: %2\n"
                   "account: %3\n")
        .arg(QString::number(id), QString::number(percent),
             QString::number(account));
  }

  size_t account;
  int percent;
};

#endif // BANK_H
