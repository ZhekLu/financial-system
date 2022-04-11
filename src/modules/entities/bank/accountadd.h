#ifndef ACCOUNTADD_H
#define ACCOUNTADD_H

#include "credit.h"
#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

#include <QDate>

class AccountAdd : public ISystemObject {
public:
  enum Type { SALARY, DEPOSIT };

  AccountAdd(size_t id, bool approved, Type type, size_t initiator_id,
             size_t bank_id, size_t account_id, QDate start_date,
             size_t period_in_month, size_t payment, size_t percent,
             size_t payed_num);

  AccountAdd(size_t initiator_id, size_t bank_id, size_t account_id,
             QDate start_date, size_t period_in_month, size_t sum); // Salary

  AccountAdd(size_t initiator_id, size_t bank_id, size_t account_id,
             QDate start_date, size_t period_in_month, size_t start_sum,
             size_t percent); // Deposit

  QString get_values_query() override;
  QString get_info() const override;

private:
  bool approved;
  Type type;
  size_t initiator_id;
  size_t bank_id;
  size_t account_id;
  QDate start_date;
  size_t period;
  size_t payment;
  size_t percent;
  size_t payed_num;

  static inline IdGenerator id_creator{9, "account_adds", "id"};
  static inline std::vector<QString> type_string{"SALARY", "DEPOSIT"};
};

#endif // ACCOUNTADD_H
