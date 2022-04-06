#ifndef USERDB_H
#define USERDB_H

#include <QList>
#include <QObject>
#include <set>
#include <unordered_map>
#include <vector>

#include "database.h"
#include "modules/entities/bank/bank.h"
#include "modules/entities/bank/bankaccount.h"
#include "modules/entities/bank/credit.h"
#include "modules/entities/bank/request.h"
#include "modules/entities/bank/transaction.h"
#include "modules/entities/entity.h"
#include "modules/entities/individual.h"
#include "modules/entities/systemuser.h"
#include "modules/singleton.h"

class UserDB : public DataBase {
  Q_OBJECT
public:
  explicit UserDB(QString dbFIlename = "users.db", QObject *parent = 0);

  // Login
  bool is_login_busy(QString login);
  bool contains(SystemUser user);
  size_t get_user_by_login(SystemUser user);

  void login_user(SystemUser user);
  void remove_user(std::string login);
  //  std::vector<SystemUser> users();

  // Users
  Individual *get_user(size_t id);

  // Companies
  void add_company(Entity company);
  void remove_company(size_t id);
  Entity *get_company(size_t id);

  // Banks
  Bank *get_bank(size_t id);
  std::vector<Bank *> get_banks();
  std::unordered_map<size_t, std::unique_ptr<Bank>> get_hash_banks();

  // Bank accounts
  bool add_account(BankAccount *);
  size_t get_account_balance(size_t id);
  BankAccount *get_account(size_t id);
  bool contains(BankAccount &acc);
  std::vector<std::unique_ptr<BankAccount>> get_user_accounts(size_t user_id);
  bool update(BankAccount &);

  // Requests
  void add_request(Request &);
  std::vector<std::unique_ptr<Request>> get_requests();
  std::vector<std::unique_ptr<Request>> get_requests(Request::Type);
  std::vector<std::unique_ptr<Request>> get_requests(Request::Type type,
                                                     bool viewed);
  std::vector<std::unique_ptr<Request>>
  get_transfer_requests(bool viewed = false);
  bool update(Request &r);

  // Transactions
  void add_transaction(Transaction &);
  std::unique_ptr<Transaction> get_transaction(size_t id);
  std::vector<std::unique_ptr<Transaction>> get_transactions();

  // Credits
  bool add_credit(Credit &);
  bool add_loan(Loan &);
  std::unique_ptr<Credit> get_credit(size_t id);
  std::vector<std::unique_ptr<Credit>> get_credits();
  bool update(Credit &);

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
