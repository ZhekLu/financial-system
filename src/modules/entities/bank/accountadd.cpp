#include "accountadd.h"

AccountAdd::AccountAdd(size_t id, bool approved, Type type, size_t initiator_id,
                       size_t bank_id, size_t account_id, QDate start_date,
                       size_t period_in_month, size_t payment, size_t percent,
                       size_t payed_num)
    : ISystemObject(id), approved(approved), type(type),
      initiator_id(initiator_id), bank_id(bank_id), account_id(account_id),
      start_date(start_date), period(period_in_month), payment(payment),
      percent(percent), payed_num(payed_num)

{}

AccountAdd::AccountAdd(size_t initiator_id, size_t bank_id, size_t account_id,
                       QDate start_date, size_t period_in_month, size_t sum)
    : AccountAdd(id_creator.GenerateId(), false, Type::SALARY, initiator_id,
                 bank_id, account_id, start_date, period_in_month, sum, 0, 0) {}

AccountAdd::AccountAdd(size_t initiator_id, size_t bank_id, size_t account_id,
                       QDate start_date, size_t period_in_month,
                       size_t start_sum, size_t percent)
    : AccountAdd(id_creator.GenerateId(), false, Type::DEPOSIT, initiator_id,
                 bank_id, account_id, start_date, period_in_month,
                 Credit::count_payment(start_sum, period_in_month, percent),
                 percent, 0) {}

QString AccountAdd::get_values_query() {
  // (id, type, approved, user_id, bank_id, account_id, start_date, period,
  // payment, percent, payed_num)
  return QString("(%1, %2, %3, %4, %5, %6, \"%7\", %8, %9, %10, %11)")
      .arg(QString::number(id), QString::number(approved),
           QString::number(type), QString::number(initiator_id),
           QString::number(bank_id), QString::number(account_id),
           start_date.toString("yyyy-MM-dd"), QString::number(period),
           QString::number(payment), QString::number(percent),
           QString::number(payed_num));
}

QString AccountAdd::get_info() const {
  return QString("Sum: %1\n"
                 "Initiator: %2\n"
                 "Approved: %3\n"
                 "Date: %4\n"
                 "Period: %5\n"
                 "Payed: %6\n")
      .arg(QString::number(payment), QString::number(initiator_id),
           approved ? "True" : "False", start_date.toString(),
           QString::number(period), QString::number(payed_num));
}

QString AccountAdd::get_update_query() const {
  return QString("approved = %1, payed_num = %2")
      .arg(QString::number(approved), QString::number(payed_num));
}

size_t AccountAdd::pay() {
  if (payed_num == period)
    return 0;
  QDate current = QDate::currentDate();
  QDate last = last_payment_date().addMonths(1);
  size_t to_pay = 0;
  while (last < current) {
    to_pay += payment;
    payed_num++;
    last = last.addMonths(1);
  }
  return to_pay;
}

QDate AccountAdd::last_payment_date() const {
  return start_date.addMonths(payed_num);
}
