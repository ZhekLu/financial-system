#ifndef INSTALLMENT_H
#define INSTALLMENT_H

#include "loan.h"
#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

#include <QDate>
#include <cmath>

class Installment : public Loan {
public:
  Installment(size_t id, bool is_opened, size_t user_id, size_t finally_sum,
              QDate start, size_t duration, size_t monthly_payment,
              size_t payed_num);

  Installment(size_t user_id, size_t start_sum, size_t month_count);
};

inline Installment::Installment(size_t id, bool is_opened, size_t user_id,
                                size_t start_sum, QDate start, size_t duration,
                                size_t monthly_payment, size_t payed_num)
    : Loan(id, is_opened, user_id, start_sum, 0, start, monthly_payment,
           payed_num, duration) {}

inline Installment::Installment(size_t user_id, size_t start_sum,
                                size_t month_count)
    : Installment(id_creator.GenerateId(), false, user_id, start_sum,
                  QDate::currentDate(), month_count, 0, 0) {
  // logic of counting payment
  double rate = percent / 100.;
  double finally = start_sum * rate;
  payment = round(finally / month_count);
  finally_sum = payment * month_count;
}

#endif // INSTALLMENT_H
