#ifndef REQUEST_H
#define REQUEST_H

#include "ISystemObject.h"

class Request : public ISystemObject {
public:
  enum Type {
    // working with bank accounts
    TRANSFER,
    FREEZE,
    WITHDRAW,
    CREDIT,
    INSTALLMENT,
    // loging
    LOGIN_ACCOUNT,
    LOGIN_USER
  };

  Request(size_t id, Type type, bool approved, size_t from, size_t to = 0)
      : ISystemObject(id), type(type), is_approved(approved), initiator(from),
        receiver(to) {}
  Request(Type type, size_t from, size_t to = 0)
      : Request(id_creator++, type, false, from, to) {}

  // id, type, sender, receiver, approved
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4, %5)")
        .arg(QString::number(id), QString::number(type),
             QString::number(initiator), QString::number(receiver),
             QString::number(is_approved));
  }

  QString get_info() const override { return {}; }

  Type type;
  bool is_approved;
  size_t initiator;                 // initiator account id (from)
  [[maybe_unused]] size_t receiver; // receiving account id (to)

private:
  static inline size_t id_creator = 0;
};

#endif // REQUEST_H
