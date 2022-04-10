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
    VERIFY,
    // block
    BLOCK,
    UNBLOCK
  };

  Request(size_t id, Type type, size_t from, size_t object,
          bool approved = false, bool viewed = false);
  Request(Type type, size_t from, size_t object);

  // id, type, sender, object, approved, viewed
  QString get_values_query() override;
  QString get_update_query() const;
  QString get_info() const override;

  // setters
  void set_approved(bool value) { approved = value; }
  void set_viewed(bool value) { viewed = value; }

  // getters
  bool is_approved() const { return approved; }
  bool is_viewed() const { return viewed; }
  size_t get_object() const { return object_id; }

private:
  Type type;
  bool approved;
  bool viewed;
  size_t sender_id; // initiator account id (from)
  size_t object_id; // what is under request

  static inline IdGenerator id_creator{9, "requests", "id"};
  static inline std::vector<QString> type_string{
      "TRANSFER",    "WITHDRAW",      "TOP UP",     "FREEZE", "CREDIT",
      "INSTALLMENT", "LOGIN_ACCOUNT", "LOGIN_USER", "UNDO",   "VERIFY"};
};

inline Request::Request(size_t id, Type type, size_t from, size_t object,
                        bool approved, bool viewed)
    : ISystemObject(id), type(type), approved(approved), viewed(viewed),
      sender_id(from), object_id(object) {}

inline Request::Request(Type type, size_t from, size_t object)
    : Request(id_creator.GenerateId(), type, from, object) {}

inline QString Request::get_values_query() {
  return QString("(%1, %2, %3, %4, %5, %6)")
      .arg(QString::number(id), QString::number(type),
           QString::number(sender_id), QString::number(object_id),
           QString::number(approved), QString::number(viewed));
}

inline QString Request::get_update_query() const {
  return QString("approved = %1, viewed = %2")
      .arg(QString::number(approved), QString::number(viewed));
}

inline QString Request::get_info() const {
  return QString("Type: %1\n").arg(type_string[type]);
}

#endif // REQUEST_H
