#ifndef REQUEST_H
#define REQUEST_H

#include "modules/database/idgenerator.h"
#include "modules/entities/ISystemObject.h"

class Request : public ISystemObject {
public:
  enum Type {
    // working with bank accounts
    TRANSFER,
    WITHDRAW,
    TOPUP,
    FREEZE,
    CREDIT,
    INSTALLMENT,
    // loging
    LOGIN_ACCOUNT,
    LOGIN_USER,
    // system
    UNDO,
    VERIFY
  };

  Request(size_t id, Type type, size_t from, size_t object,
          bool approved = false, bool viewed = false)
      : ISystemObject(id), type(type), is_approved(approved), viewed(viewed),
        sender_id(from), object_id(object) {}
  Request(Type type, size_t from, size_t object)
      : Request(id_creator.GenerateId(), type, from, object) {}

  // id, type, sender, object, approved, viewed
  QString get_values_query() override {
    return QString("(%1, %2, %3, %4, %5, %6)")
        .arg(QString::number(id), QString::number(type),
             QString::number(sender_id), QString::number(object_id),
             QString::number(is_approved), QString::number(viewed));
  }

  QString get_info() const override {
    return QString("Type: %1\n").arg(type_string[type]);
  }

  Type type;
  bool is_approved;
  bool viewed;
  size_t sender_id; // initiator account id (from)
  size_t object_id; // what is under request

private:
  static inline IdGenerator id_creator{9, "requests", "id"};
  static inline std::vector<QString> type_string{
      "TRANSFER",    "WITHDRAW",      "TOP UP",     "FREEZE", "CREDIT",
      "INSTALLMENT", "LOGIN_ACCOUNT", "LOGIN_USER", "UNDO",   "VERIFY"};
};

#endif // REQUEST_H
