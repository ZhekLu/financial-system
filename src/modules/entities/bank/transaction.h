#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

class Transaction : public ISystemObject {
public:
  enum Type { TRANSFER, WITHDRAW, TOPUP };

  Transaction(Type type, size_t id, size_t from, size_t to, size_t sum,
              bool approved = false)
      : ISystemObject(id), approved(approved), sender(from), receiver(to),
        amount(sum), type(type) {}
  Transaction(size_t from, size_t to, size_t sum)
      : Transaction(Type::TRANSFER, id_creator.GenerateId(), from, to, sum) {}
  Transaction(size_t from, size_t sum, bool minus = true)
      : Transaction(from, 0, sum) {
    if (minus)
      type = Type::WITHDRAW;
    else
      type = Type::TOPUP;
  }

  // getters
  bool is_approved() const { return approved; }
  size_t get_sender() const { return sender; }
  size_t get_receiver() const { return receiver; }
  size_t get_amount() const { return amount; }
  Type get_type() const { return type; }

  // setters
  void set_approved(bool value) { approved = value; }

  // id, sender, receiver, amount, approved, type
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4, %5, %6)")
        .arg(QString::number(id), QString::number(sender),
             QString::number(receiver), QString::number(amount),
             QString::number(approved), QString::number(type));
  }

  QString get_info() const override {
    return QString("Sender: %1\n"
                   "Receiver: %2\n"
                   "Sum: %3\n"
                   "approved: %4\n")
        .arg(QString::number(sender), QString::number(receiver),
             QString::number(amount), approved ? "True" : "False");
  }

private:
  bool approved;
  size_t sender;   // initiator account id (from)
  size_t receiver; // receiving account id (to)
  size_t amount;
  Type type;

  static inline IdGenerator id_creator{9, "transactions", "id"};
};

#endif // TRANSACTION_H
