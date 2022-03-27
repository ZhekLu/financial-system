#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

class Transaction : public ISystemObject {
public:
  Transaction(size_t id, size_t from, size_t to, size_t sum,
              bool approved = false)
      : ISystemObject(id), is_approved(approved), sender(from), receiver(to),
        amount(sum) {}
  Transaction(size_t from, size_t to, size_t sum)
      : Transaction(id_creator.GenerateId(), from, to, sum) {}

  // id, sender, receiver, amount, approved
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4, %5)")
        .arg(QString::number(id), QString::number(sender),
             QString::number(receiver), QString::number(amount),
             QString::number(is_approved));
  }

  QString get_info() const override { return {}; }

  bool is_approved;
  size_t sender;   // initiator account id (from)
  size_t receiver; // receiving account id (to)
  size_t amount;

private:
  static inline IdGenerator id_creator{9, "transactions", "id"};
};

#endif // TRANSACTION_H
