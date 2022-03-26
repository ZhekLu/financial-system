#ifndef USERDB_H
#define USERDB_H

#include <QList>
#include <QObject>
#include <set>
#include <vector>

#include "database.h"
#include "modules/entities/systemuser.h"
#include "modules/entities/user.h"
#include "modules/singleton.h"

class UserDB : public DataBase {
  Q_OBJECT
public:
  explicit UserDB(QString dbFIlename = "users.db", QObject *parent = 0);

  // Login
  bool is_login_busy(QString login);

  bool contains(SystemUser user);
  size_t get_id_by_login(SystemUser user);

  void login_user(SystemUser user);
  void remove_user(std::string login);
  //  std::vector<SystemUser> users();

  // Debug methods
  void print_all_system_users();
  void print_mode_system_users(LoginMode);

  // Users
  User *get_user(size_t id);

  //  // Bank accounts
  //  void add_account(BankAccount product);

  //  // Companies
  //  void add_company(Company company);
  //  void remove_company(size_t id);

  void test_login();
  void test_users();

protected:
  bool createTables() override;
public slots:
};

#define USER_DB Singleton<UserDB>::GetInstance()
#endif // USERDB_H
