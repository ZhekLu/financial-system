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

  Request(size_t id, Type type, size_t from, bool approved = false)
      : ISystemObject(id), type(type), is_approved(approved), initiator(from) {}
  Request(Type type, size_t from)
      : Request(id_creator.GenerateId(), type, from) {}

  // id, type, sender, approved
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4)")
        .arg(QString::number(id), QString::number(type),
             QString::number(initiator), QString::number(is_approved));
  }

  QString get_info() const override { return {}; }

  Type type;
  bool is_approved;
  size_t initiator; // initiator account id (from)

private:
  static inline IdGenerator id_creator{9, "requests", "id"};
};

#endif // REQUEST_H
