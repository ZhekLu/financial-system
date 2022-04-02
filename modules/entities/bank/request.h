#ifndef REQUEST_H
#define REQUEST_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

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

  Request(size_t id, Type type, size_t from, size_t object,
          bool approved = false)
      : ISystemObject(id), type(type), is_approved(approved), sender_id(from),
        object(object) {}
  Request(Type type, size_t from, size_t object)
      : Request(id_creator.GenerateId(), type, from, object) {}

  // id, type, sender, approved
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4)")
        .arg(QString::number(id), QString::number(type),
             QString::number(sender_id), QString::number(is_approved));
  }

  QString get_info() const override { return {}; }

  Type type;
  bool is_approved;
  size_t sender_id; // initiator account id (from)
  size_t object;    // what is under request (id)

private:
  static inline IdGenerator id_creator{9, "requests", "id"};
};

#endif // REQUEST_H
