#ifndef CREDIT_H
#define CREDIT_H

#include "loan.h"
#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

#include <QDate>
#include <cmath>

class Credit : public Loan {
public:
  Credit(size_t id, bool is_opened, size_t user_id, size_t start_sum,
         size_t percent, QDate start, size_t duration, size_t monthly_payment,
         size_t payed_num);

  Credit(size_t user_id, size_t start_sum, size_t percent, size_t month_count);
};

inline Credit::Credit(size_t id, bool is_opened, size_t user_id,
                      size_t start_sum, size_t percent, QDate start,
                      size_t duration, size_t monthly_payment, size_t payed_num)
    : Loan(id, is_opened, user_id, start_sum, percent, start, monthly_payment,
           payed_num, duration) {}

inline Credit::Credit(size_t user_id, size_t start_sum, size_t percent,
                      size_t month_count)
    : Credit(id_creator.GenerateId(), false, user_id, start_sum, percent,
             QDate::currentDate(), month_count, 0, 0) {
  // logic of counting payment
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

#endif // CREDIT_H
