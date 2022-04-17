#ifndef SYSTEMUSER_H
#define SYSTEMUSER_H

#include "ISystemObject.h"
#include "modules/database/idgenerator.h"
#include "modules/modes.h"
#include <string>

struct SystemUser : public ISystemObject {
  SystemUser(size_t id, std::string login, std::string password, LoginMode lm,
             size_t user_id, bool approved)
      : ISystemObject(id), login(std::move(login)),
        password(std::move(password)), mode(lm), user_id(user_id),
        approved(approved) {}

  SystemUser(QString login, QString password, LoginMode lm, size_t user_id = 0)
      : SystemUser(id_creator.GenerateId(), login.toStdString(),
                   password.toStdString(), lm, user_id, false) {}

  QString get_values_query() override;
  QString get_update_query() const;
  QString get_info() const override;

  LoginMode get_role() const { return mode; }
  QString get_login() const { return QString::fromStdString(login); }
  size_t get_user() const { return user_id; }
  bool is_available() const { return approved; }

  bool check_password(QString &password) const {
    return get_password() == password;
  }
  void set_approved(bool value) { approved = value; }
  void set_user(size_t id) { user_id = id; }

private:
  QString get_password() const { return QString::fromStdString(password); }

  // fields
  std::string login;
  std::string password;
  LoginMode mode;
  size_t user_id;
  bool approved;

  static inline IdGenerator id_creator{9, "system_users", "id"};
};

inline QString SystemUser::get_values_query() {
  // id, login, password, role, user_id, approved
  return QString("(%1, '%2', '%3', %4, %5, %6)")
      .arg(QString::number(id), get_login(), get_password(),
           QString::number(mode), QString::number(user_id),
           QString::number(approved));
}

inline QString SystemUser::get_update_query() const {
  return QString("login = '%1', password = '%2', role = %3, approved = %4")
      .arg(get_login(), get_password(), QString::number(mode),
           QString::number(approved));
}

inline QString SystemUser::get_info() const {
  return QString("Login: %1\n"
                 "Role: %2\n")
      .arg(get_login(), QString::number(mode));
}

#endif // SYSTEMUSER_H
