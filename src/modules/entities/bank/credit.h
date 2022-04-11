#ifndef CREDIT_H
#define CREDIT_H

#include "loan.h"
#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

#include <QDate>
#include <cmath>

class Credit : public Loan {
public:
  static size_t count_payment(size_t sum, size_t period_in_month,
                              size_t percent);

  Credit(size_t id, bool is_opened, size_t user_id, size_t start_sum,
         size_t percent, QDate start, size_t duration, size_t monthly_payment,
         size_t payed_num);

  Credit(size_t user_id, size_t start_sum, size_t percent, size_t month_count);
};

inline size_t Credit::count_payment(size_t sum, size_t period_in_month,
                                    size_t percent) {
  double rate = percent / 100. / 12.;
  double finally = sum;
  for (size_t i = 0; i < period_in_month; i++)
    finally += finally * rate;
  size_t payment = round(finally / period_in_month);
  return payment;
}

inline Credit::Credit(size_t id, bool is_opened, size_t user_id,
                      size_t start_sum, size_t percent, QDate start,
                      size_t duration, size_t monthly_payment, size_t payed_num)
    : Loan(id, is_opened, user_id, start_sum, percent, start, duration,
           monthly_payment, payed_num) {}

inline Credit::Credit(size_t user_id, size_t start_sum, size_t percent,
                      size_t month_count)
    : Credit(id_creator.GenerateId(), false, user_id, start_sum, percent,
             QDate::currentDate(), month_count, 0, 0) {
  payment = count_payment(start_sum, period, percent);
  finally_sum = payment * month_count;
}

#endif // CREDIT_H
