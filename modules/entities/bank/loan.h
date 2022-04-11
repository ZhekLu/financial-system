#ifndef LOAN_H
#define LOAN_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

#include <QDate>
#include <cmath>

class Loan : public ISystemObject {
public:
  // TODO? enum Type {};
  Loan(size_t id, bool is_opened, size_t user_id, size_t start_sum,
       size_t percent, QDate start, size_t duration, size_t monthly_payment,
       size_t payed_num);

  Loan(size_t user_id, size_t start_sum, size_t percent, size_t month_count);

  // id, opened, user_id, start_sum, percent,
  // start_date, period, payment, payed_num
  QString get_values_query() override;
  QString get_update_query() const;
  QString get_info() const override;

  void set_open(bool value);
  void pay_fee() { payed_num++; }

  size_t get_start_sum() const { return start_sum; }
  size_t get_payment() const { return payment; }

protected:
  bool opened;
  size_t user_id;
  size_t start_sum;
  size_t percent;
  QDate start_date;
  size_t payment;
  size_t payed_num;
  size_t period;
  size_t finally_sum = 0;

  static inline IdGenerator id_creator{9, "credits", "id"};
};

inline Loan::Loan(size_t id, bool is_opened, size_t user_id, size_t start_sum,
                  size_t percent, QDate start, size_t duration,
                  size_t monthly_payment, size_t payed_num)
    : ISystemObject(id), opened(is_opened), user_id(user_id),
      start_sum(start_sum), percent(percent), start_date(start),
      payment(monthly_payment), payed_num(payed_num), period(duration) {}

inline Loan::Loan(size_t user_id, size_t start_sum, size_t percent,
                  size_t month_count)
    : Loan(id_creator.GenerateId(), false, user_id, start_sum, percent,
           QDate::currentDate(), month_count, 0, 0) {
  // logic of counting payment : in children classes
}

inline QString Loan::get_values_query() {
  return QString("(%1, %2, %3, %4, %5, \"%6\", %7, %8, %9)")
      .arg(QString::number(id), QString::number(opened),
           QString::number(user_id), QString::number(start_sum),
           QString::number(percent), start_date.toString("yyyy-MM-dd"),
           QString::number(period), QString::number(payment),
           QString::number(payed_num));
}

inline QString Loan::get_update_query() const {
  return QString("opened = %1, start_date = '%2', payed_num = %3")
      .arg(QString::number(opened), start_date.toString("yyyy-MM-dd"),
           QString::number(payed_num));
}

inline QString Loan::get_info() const {
  return QString("User: %1\n"
                 "Sum: %2\n"
                 "Opened: %3\n"
                 "Date: %4\n"
                 "Period: %5\n"
                 "Payed: %6\n")
      .arg(QString::number(user_id), QString::number(start_sum),
           opened ? "True" : "False", start_date.toString(),
           QString::number(period), QString::number(payed_num));
}

inline void Loan::set_open(bool value) {
  opened = value;
  if (value)
    start_date = QDate::currentDate();
}

#endif // LOAN_H
