#ifndef CREDIT_H
#define CREDIT_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

#include <QDate>
#include <cmath>

class Credit : public ISystemObject {
public:
  Credit(size_t id, bool is_opened, size_t user_id, size_t start_sum,
         size_t percent, QDate start, QDate end, size_t monthly_payment,
         size_t payed_num)
      : ISystemObject(id), opened(is_opened), user_id(user_id),
        start_sum(start_sum), percent(percent), start_date(start),
        end_date(end), payment(monthly_payment), payed_num(payed_num) {}

  Credit(size_t user_id, size_t start_sum, size_t percent, size_t month_count)
      : Credit(id_creator.GenerateId(), false, user_id, start_sum, percent,
               QDate::currentDate(),
               QDate::currentDate().addMonths(month_count), 0, 0) {
    // logic of counting payment
    // TODO!
    period = month_count;
    int years_num = month_count / 12;
    double rate = percent / 100.;
    double finally = start_sum;
    for (int i = 0; i < years_num; i++) {
      finally += finally * rate;
    }
    payment = round(finally / month_count);
    finally_sum = payment * month_count;
  }

  // id, opened, user_id, start_sum, percent,
  // start_date, end_date, payment, payed_num
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4, %5, \"%6\", \"%7\", %8, %9)")
        .arg(QString::number(id), QString::number(opened),
             QString::number(user_id), QString::number(start_sum),
             QString::number(percent), start_date.toString("yyyy-MM-dd"),
             end_date.toString("yyyy-MM-dd"), QString::number(payment),
             QString::number(payed_num));
  }

  QString get_info() const override {
    return QString("User: %1\n"
                   "Sum: %2\n"
                   "Opened: %3\n"
                   "Date: %4\n"
                   "Period: %5\n")
        .arg(QString::number(user_id), QString::number(start_sum),
             opened ? "True" : "False", start_date.toString(),
             QString::number(period));
  }

  bool opened;
  size_t user_id;
  size_t start_sum;
  size_t percent;
  QDate start_date;
  QDate end_date;
  size_t payment;
  size_t payed_num;
  size_t period = 0;
  size_t finally_sum = 0;

private:
  static inline IdGenerator id_creator{9, "credits", "id"};
};

#endif // CREDIT_H
