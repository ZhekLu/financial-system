#ifndef USERDB_H
#define USERDB_H

#include <QList>
#include <QObject>
#include <set>
#include <vector>

#include "database.h"
#include "modules/entities/bankaccount.h"
#include "modules/entities/entity.h"
#include "modules/entities/individual.h"
#include "modules/entities/request.h"
#include "modules/entities/systemuser.h"
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

  // Users
  Individual *get_user(size_t id);

  // Companies
  void add_company(Entity company);
  void remove_company(size_t id);
  Entity *get_company(size_t id);

  // Bank accounts
  void add_account(BankAccount);
  size_t get_account_balance(size_t id);
  BankAccount *get_account(size_t id);
  bool contains(BankAccount &acc);
  std::vector<BankAccount *> get_user_accounts(size_t user_id);
  // Update methods
  bool update(BankAccount &);

  // Requests
  void add_request(Request &);

  // Debug methods
  void print_all_system_users();
  void print_mode_system_users(LoginMode);

  // Fill tables
  void test_login();
  void test_users();

protected:
  bool createTables() override;
public slots:
};

#define USER_DB Singleton<UserDB>::GetInstance()
#endif // USERDB_H
