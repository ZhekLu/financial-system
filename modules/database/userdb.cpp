#include "userdb.h"

UserDB::UserDB(QString dbFIlename, QObject *parent)
    : DataBase(dbFIlename, parent) {
  if (!connectToDataBase())
    qDebug() << "FAULT";
}

bool UserDB::createTables() {

  QSqlQuery query;

  bool res = true;
  if (!db.tables().contains("users")) {
    res = query.exec("CREATE TABLE users"
                     "("
                     "id INTEGER PRIMARY KEY,"
                     "full_name TEXT, "
                     "pass_number TEXT, "
                     "pass_id TEXT,"
                     "phone TEXT,"
                     "email TEXT"
                     ");");
  }

  if (!db.tables().contains("system_users") && res) {
    res = query.exec("CREATE TABLE system_users"
                     "("
                     "login TEXT PRIMARY KEY, "
                     "password TEXT, "
                     "role INTEGER, "
                     "user_id INTEGER"
                     ");");
  }

  if (!db.tables().contains("system_banks") && res) {
    res = query.exec("CREATE TABLE system_banks"
                     "("
                     "BIC INTEGER PRIMARY KEY,"
                     "precent INTEGER"
                     ");");
  }

  if (!db.tables().contains("accounts") && res) {
    res = query.exec("CREATE TABLE accounts"
                     "("
                     "id INTEGER PRIMARY KEY, "
                     "user_id INTEGER, "
                     "bank_id INTEGER, "
                     "balance INTEGER,"
                     "frozen BOOLEAN"
                     ");");
  }

  if (!db.tables().contains("companies") && res) {
    res = query.exec("CREATE TABLE companies"
                     "("
                     "name TEXT, "
                     "type INTEGER, "
                     "PAC INTEGER, "
                     "BIC INTEGER PRIMARY KEY, "
                     "adress TEXT,"
                     "bank_bic INTEGER"
                     ");");
  }

  if (!db.tables().contains("requests") && res) {
    res = query.exec("CREATE TABLE requests"
                     "("
                     "id INTEGER PRIMARY KEY, "
                     "type INTEGER, "
                     "sender INTEGER, "
                     "object INTEGER, "
                     "approved BOOLEAN, "
                     "viewed BOOLEAN"
                     ");");
  }

  if (!db.tables().contains("transactions") && res) {
    res = query.exec("CREATE TABLE transactions"
                     "("
                     "id INTEGER PRIMARY KEY, "
                     "sender INTEGER, "
                     "receiver INTEGER,"
                     "amount INTEGER,"
                     "approved BOOLEAN,"
                     "type INTEGER"
                     ");");
  }

  if (!db.tables().contains("credits") && res) {
    res = query.exec("CREATE TABLE credits"
                     "("
                     "id INTEGER PRIMARY KEY, "
                     "opened BOOLEAN, "
                     "user_id INTEGER, "
                     "start_sum INTEGER,"
                     "percent INTEGER,"
                     "start_date DATE,"
                     "period INTEGER, "
                     "payment INTEGER, "
                     "payed_num INTEGER "
                     ");");
  }

  return res;
}

// Login and system

bool UserDB::is_login_busy(QString login) {
  QString query =
      tr("SELECT login FROM system_users WHERE login = ") + qs(login);
  exec(query);

  return db_query->first();
}

bool UserDB::contains(SystemUser user) {

  QString query = QString("SELECT user_id FROM system_users "
                          "WHERE login = \"%1\" AND "
                          "password = \"%2\" AND "
                          "role = %3")
                      .arg(QString::fromStdString(user.login))
                      .arg(QString::fromStdString(user.password))
                      .arg(QString::number(user.mode));
  exec(query);
  return db_query->next();
}

size_t UserDB::get_user_by_login(SystemUser user) {
  QString query = QString("SELECT user_id FROM system_users "
                          "WHERE login = \"%1\" AND "
                          "password = \"%2\" AND "
                          "role = %3")
                      .arg(QString::fromStdString(user.login))
                      .arg(QString::fromStdString(user.password))
                      .arg(QString::number(user.mode));
  exec(query);
  if (!db_query->next())
    return 0;
  return db_query->value(0).toInt();
}

void UserDB::login_user(SystemUser user) {
  QString query = "INSERT INTO system_users(login, password, role, "
                  "user_id) VALUES (" +
                  qs(QString::fromStdString(user.login)) + "," +
                  qs(QString::fromStdString(user.password)) + "," +
                  qs(QString::number(user.mode)) + "," +
                  qs(QString::number(user.user_id)) + ");";
  if (exec(query))
    emit DataBase::updated();
}

// User

Individual *UserDB::get_user(size_t id) {
  QString query = ("SELECT full_name, pass_number, "
                   "pass_id, phone, email "
                   "FROM users WHERE id = ") +
                  QString::number(id);
  exec(query);

  if (!db_query->next())
    return nullptr;

  std::string name = db_query->value(0).toString().toStdString();
  std::string p_number = db_query->value(1).toString().toStdString();
  std::string p_id = db_query->value(2).toString().toStdString();
  std::string phone = db_query->value(3).toString().toStdString();
  std::string email = db_query->value(4).toString().toStdString();

  return new Individual(name, p_number, p_id, phone, email, id);
}

void UserDB::remove_user(std::string login) {
  QString query = ("DELETE FROM users WHERE login = ") + qs(login);
  if (exec(query))
    emit DataBase::updated();
}

// Companies

void UserDB::add_company(Entity company) {
  QString query =
      "INSERT INTO companies(id, name, type, PAC, BIC, adress, bank_bic) "
      "VALUES" +
      QString::number(company.get_id()) + "," + qs(company.get_name()) + "," +
      QString::number(company.type) + "," + QString::number(company.PAC) + "," +
      QString::number(company.BIC) + "," + qs(company.adress) + "," +
      QString::number(company.bank_bic) + ");";
  if (exec(query))
    emit DataBase::updated();
}

void UserDB::remove_company(size_t id) {
  QString query = tr("DELETE FROM companies WHERE id = ") + QString::number(id);
  if (exec(query))
    emit DataBase::updated();
}

Entity *UserDB::get_company(size_t id) {
  QString query = ("SELECT name, type, PAC, adress, bank_bic "
                   "FROM companies WHERE BIC = ") +
                  QString::number(id);
  exec(query);

  if (!db_query->next())
    return nullptr;

  std::string name = db_query->value(0).toString().toStdString();
  int type = db_query->value(1).toInt();
  size_t PAC = db_query->value(2).toULongLong();
  std::string adress = db_query->value(3).toString().toStdString();
  size_t bank_bic = db_query->value(4).toULongLong();

  return new Entity(Entity::Type(type), name, PAC, id, adress, bank_bic);
}

Bank *UserDB::get_bank(size_t id) {
  QString query = ("SELECT percent, name, type, PAC, adress, bank_bic "
                   "FROM system_banks WHERE BIC = ") +
                  QString::number(id);
  exec(query);

  if (!db_query->next())
    return nullptr;

  int percent = db_query->value(0).toInt();
  std::string name = db_query->value(1).toString().toStdString();
  int type = db_query->value(2).toInt();
  size_t PAC = db_query->value(3).toULongLong();
  std::string adress = db_query->value(4).toString().toStdString();
  size_t bank_bic = db_query->value(5).toULongLong();

  return new Bank(percent, Entity::Type(type), name, PAC, id, adress, bank_bic);
}

std::vector<Bank *> UserDB::get_banks() {
  std::vector<Bank *> banks;
  QString query = ("SELECT BIC, percent, name, type, PAC, adress, bank_bic "
                   "FROM system_banks");
  exec(query);

  while (db_query->next()) {
    size_t BIC = db_query->value(0).toULongLong();
    int percent = db_query->value(1).toInt();
    std::string name = db_query->value(2).toString().toStdString();
    int type = db_query->value(3).toInt();
    size_t PAC = db_query->value(4).toULongLong();
    std::string adress = db_query->value(5).toString().toStdString();
    size_t bank_bic = db_query->value(6).toULongLong();
    banks.push_back(new Bank(percent, Entity::Type(type), name, PAC, BIC,
                             adress, bank_bic));
  }

  return banks;
}

std::unordered_map<size_t, std::unique_ptr<Bank>> UserDB::get_hash_banks() {
  std::unordered_map<size_t, std::unique_ptr<Bank>> banks;
  QString query = ("SELECT BIC, percent, name, type, PAC, adress, bank_bic "
                   "from system_banks "
                   "left join companies using (BIC)");
  exec(query);

  while (db_query->next()) {
    size_t BIC = db_query->value(0).toULongLong();
    int percent = db_query->value(1).toInt();
    std::string name = db_query->value(2).toString().toStdString();
    int type = db_query->value(3).toInt();
    size_t PAC = db_query->value(4).toULongLong();
    std::string adress = db_query->value(5).toString().toStdString();
    size_t bank_bic = db_query->value(6).toULongLong();
    banks[BIC] = std::make_unique<Bank>(percent, Entity::Type(type), name, PAC,
                                        BIC, adress, bank_bic);
  }

  return banks;
}

// Bank accounts

bool UserDB::add_account(BankAccount *acc) {
  QString query = "INSERT INTO accounts(id, user_id, bank_id, balance, frozen) "
                  "VALUES " +
                  acc->get_values_query();
  if (exec(query)) {
    emit DataBase::updated();
    return true;
  }
  return false;
}

size_t UserDB::get_account_balance(size_t id) {
  QString query =
      ("SELECT balance FROM accounts WHERE id = ") + qs(QString::number(id));
  exec(query);

  if (!db_query->next())
    return 0;

  return db_query->value(0).toInt();
}

BankAccount *UserDB::get_account(size_t id) {
  QString query =
      ("SELECT user_id, bank_id, balance, frozen FROM accounts WHERE id = ") +
      QString::number(id);
  exec(query);

  if (!db_query->next())
    return nullptr;

  size_t user_id = db_query->value(0).toULongLong();
  size_t bank_id = db_query->value(1).toULongLong();
  size_t balance = db_query->value(2).toULongLong();
  bool frozen = db_query->value(3).toBool();
  return new BankAccount(id, user_id, bank_id, balance, frozen);
}

bool UserDB::contains(BankAccount &acc) {
  QString query =
      QString("SELECT * FROM accounts WHERE id = %1 and user_id = %2 and "
              "bank_id = %3 and balance = %4 and frozen = %5;")
          .arg(QString::number(acc.id), QString::number(acc.owner_id),
               QString::number(acc.bank_id), QString::number(acc.balance),
               QString::number(acc.is_frozen));
  exec(query);
  return db_query->next();
}

std::vector<std::unique_ptr<BankAccount>>
UserDB::get_user_accounts(size_t user_id) {
  QString query = ("SELECT id, bank_id, balance, frozen "
                   "FROM accounts "
                   "WHERE user_id = ") +
                  qs(QString::number(user_id));
  exec(query);

  std::vector<std::unique_ptr<BankAccount>> accounts;

  while (db_query->next()) {
    size_t id = db_query->value(0).toInt();
    size_t bank_id = db_query->value(1).toInt();
    size_t balance = db_query->value(2).toInt();
    bool frozen = db_query->value(3).toBool();
    accounts.push_back(
        std::make_unique<BankAccount>(id, user_id, bank_id, balance, frozen));
  }
  return accounts;
}

bool UserDB::update(BankAccount &acc) {
  QString query =
      QString("UPDATE accounts SET balance = %1, frozen = %2 "
              "WHERE id = %3")
          .arg(QString::number(acc.balance), QString::number(acc.is_frozen),
               QString::number(acc.id));
  if (exec(query)) {
    emit DataBase::updated();
    return true;
  }
  return false;
}

// Requests

void UserDB::add_request(Request &r) {
  QString query = QString("INSERT INTO requests"
                          "(id, type, sender, object, approved, viewed) "
                          "VALUES %1;")
                      .arg(r.get_values_query());
  if (exec(query))
    emit DataBase::updated();
}

std::vector<std::unique_ptr<Request>> UserDB::get_requests() {
  QString query = "SELECT id, type, sender, object, approved "
                  "FROM requests;";
  exec(query);

  std::vector<std::unique_ptr<Request>> requests;

  while (db_query->next()) {
    size_t id = db_query->value(0).toULongLong();
    size_t type = db_query->value(1).toInt();
    size_t sender = db_query->value(2).toULongLong();
    size_t object = db_query->value(3).toULongLong();
    bool approved = db_query->value(4).toBool();
    requests.push_back(std::make_unique<Request>(id, Request::Type(type),
                                                 sender, object, approved));
  }
  return requests;
}

std::vector<std::unique_ptr<Request>> UserDB::get_requests(Request::Type type) {
  QString query = "SELECT id, sender, object, approved "
                  "FROM requests WHERE type = " +
                  QString::number(type);
  exec(query);

  std::vector<std::unique_ptr<Request>> requests;

  while (db_query->next()) {
    size_t id = db_query->value(0).toULongLong();
    size_t sender = db_query->value(1).toULongLong();
    size_t object = db_query->value(2).toULongLong();
    bool approved = db_query->value(3).toBool();
    requests.push_back(std::make_unique<Request>(id, Request::Type(type),
                                                 sender, object, approved));
  }
  return requests;
}

std::vector<std::unique_ptr<Request>> UserDB::get_requests(Request::Type type,
                                                           bool viewed) {
  QString query = QString("SELECT id, sender, object, approved "
                          "FROM requests WHERE type = %1 and viewed = %2;")
                      .arg(QString::number(type), QString::number(viewed));
  exec(query);

  std::vector<std::unique_ptr<Request>> requests;

  while (db_query->next()) {
    size_t id = db_query->value(0).toULongLong();
    size_t sender = db_query->value(1).toULongLong();
    size_t object = db_query->value(2).toULongLong();
    bool approved = db_query->value(3).toBool();
    requests.push_back(
        std::make_unique<Request>(id, type, sender, object, approved));
  }
  return requests;
}

std::vector<std::unique_ptr<Request>>
UserDB::get_transfer_requests(bool viewed) {
  QString query = "SELECT id, type, sender, object, approved "
                  "FROM requests "
                  "WHERE type < 3 and viewed = " +
                  QString::number(viewed);
  exec(query);

  std::vector<std::unique_ptr<Request>> requests;

  while (db_query->next()) {
    size_t id = db_query->value(0).toULongLong();
    int type = db_query->value(1).toInt();
    size_t sender = db_query->value(2).toULongLong();
    size_t object = db_query->value(3).toULongLong();
    bool approved = db_query->value(4).toBool();
    requests.push_back(std::make_unique<Request>(id, Request::Type(type),
                                                 sender, object, approved));
  }
  return requests;
}

bool UserDB::update(Request &r) {
  QString query =
      QString("UPDATE requests SET viewed = %1 "
              "WHERE id = %2;")
          .arg(QString::number(r.viewed), QString::number(r.get_id()));
  //  qDebug() << query;
  if (exec(query)) {
    qDebug() << "ok";
    emit DataBase::updated();
    return true;
  }
  return false;
}

// Transactions

void UserDB::add_transaction(Transaction &t) {
  QString query = QString("INSERT INTO transactions"
                          "(id, sender, receiver, amount, approved, type) "
                          "VALUES %1;")
                      .arg(t.get_values_query());
  if (exec(query))
    emit DataBase::updated();
}

std::unique_ptr<Transaction> UserDB::get_transaction(size_t id) {
  QString query = QString("SELECT sender, receiver, amount, approved, type "
                          "FROM transactions "
                          "WHERE id = %1;")
                      .arg(QString::number(id));
  exec(query);

  if (!db_query->next())
    return nullptr;

  size_t sender = db_query->value(0).toULongLong();
  size_t receiver = db_query->value(1).toULongLong();
  size_t amount = db_query->value(2).toULongLong();
  bool approved = db_query->value(3).toBool();
  int type = db_query->value(4).toInt();
  return std::make_unique<Transaction>(Transaction::Type(type), id, sender,
                                       receiver, amount, approved);
}

std::vector<std::unique_ptr<Transaction>> UserDB::get_transactions() {
  QString query = QString("SELECT id, sender, receiver, amount, approved, type "
                          "FROM transactions;");
  exec(query);

  std::vector<std::unique_ptr<Transaction>> transactions;
  while (db_query->next()) {
    size_t id = db_query->value(0).toULongLong();
    size_t sender = db_query->value(1).toULongLong();
    size_t receiver = db_query->value(2).toULongLong();
    size_t amount = db_query->value(3).toULongLong();
    bool approved = db_query->value(4).toBool();
    int type = db_query->value(5).toInt();
    transactions.push_back(std::make_unique<Transaction>(
        Transaction::Type(type), id, sender, receiver, amount, approved));
  }
  return transactions;
}

// Credits

bool UserDB::add_credit(Credit &c) {
  QString query = QString("INSERT INTO credits"
                          "(id, opened, user_id, start_sum, percent, "
                          "start_date, period, payment, payed_num) "
                          "VALUES %1;")
                      .arg(c.get_values_query());

  if (exec(query)) {
    emit DataBase::updated();
    return true;
  }
  return false;
}

std::unique_ptr<Credit> UserDB::get_credit(size_t id) {
  QString query = QString("SELECT "
                          "opened, user_id, start_sum, percent, "
                          "start_date, period, payment, payed_num "
                          "FROM credits WHERE id = ") +
                  QString::number((id));
  exec(query);

  std::unique_ptr<Credit> credit;
  if (!db_query->next())
    return nullptr;
  bool opened = db_query->value(0).toBool();
  size_t user_id = db_query->value(1).toULongLong();
  size_t start_sum = db_query->value(2).toULongLong();
  size_t percent = db_query->value(3).toULongLong();
  QDate start_date = db_query->value(4).toDate();
  size_t period = db_query->value(5).toULongLong();
  size_t payment = db_query->value(6).toULongLong();
  size_t payed_num = db_query->value(7).toULongLong();
  credit = std::make_unique<Credit>(id, opened, user_id, start_sum, percent,
                                    start_date, period, payment, payed_num);

  return credit;
}

std::vector<std::unique_ptr<Credit>> UserDB::get_credits() {
  QString query = QString("SELECT "
                          "id, opened, user_id, start_sum, percent, "
                          "start_date, period, payment, payed_num "
                          "FROM credits;");
  exec(query);

  std::vector<std::unique_ptr<Credit>> credits;
  while (db_query->next()) {
    size_t id = db_query->value(0).toULongLong();
    bool opened = db_query->value(1).toBool();
    size_t user_id = db_query->value(2).toULongLong();
    size_t start_sum = db_query->value(3).toULongLong();
    size_t percent = db_query->value(4).toULongLong();
    QDate start_date = db_query->value(5).toDate();
    size_t period = db_query->value(6).toULongLong();
    size_t payment = db_query->value(7).toULongLong();
    size_t payed_num = db_query->value(8).toULongLong();
    credits.push_back(std::make_unique<Credit>(id, opened, user_id, start_sum,
                                               percent, start_date, period,
                                               payment, payed_num));
  }
  return credits;
}

bool UserDB::update(Credit &c) {
  QString query =
      QString(
          "UPDATE credits SET opened = %1, start_date = '%2', payed_num = %3 "
          "WHERE id = %4")
          .arg(QString::number(c.opened), c.start_date.toString("yyyy-MM-dd"),
               QString::number(c.payed_num), QString::number(c.get_id()));
  if (exec(query)) {
    emit DataBase::updated();
    return true;
  }
  return false;
}

// Debug

void UserDB::print_all_system_users() {
  QString query = "SELECT * FROM system_users";
  if (!exec(query))
    qDebug() << "UserDB::print_all_users::fail";

  qDebug() << "ALL:";
  while (db_query->next()) {
    qDebug() << "Start:" << db_query->value(0).toString()
             << db_query->value(1).toString() << db_query->value(2).toString()
             << ":End";
  }
  qDebug() << ":ALL";
}

void UserDB::print_mode_system_users(LoginMode lm) {
  QString query =
      "SELECT * FROM system_users WHERE role = " + QString::number(lm);
  if (!exec(query))
    qDebug() << "UserDB::print_all_users::fail";

  qDebug() << "ALL in this mode:";
  while (db_query->next()) {
    qDebug() << "Start:" << db_query->value(0).toString()
             << db_query->value(1).toString() << db_query->value(2).toString()
             << ":End";
  }
  qDebug() << ":ALL in this mode";
}

void UserDB::test_login() {

  QString query = "INSERT INTO system_users (login, password, role, user_id)"
                  "VALUES "
                  "(\"login1\", \"pass1\", 1, 1),"
                  "(\"login2\", \"pass2\", 2, 2),"
                  "(\"login3\", \"pass3\", 3, 13);";
  if (exec(query))
    qDebug() << "testloginOK";
  else
    qDebug() << "testloginNO";
}

void UserDB::test_users() {
  QString query =
      "INSERT INTO users (id, full_name, pass_number, phone, email)"
      "VALUES "
      "(1, \"name first manager\", \"pas_num1\", \"phone1\", \"email1\"),"
      "(2, \"name first operator\", \"pas_num2\", \"phone2\", \"email2\"),"
      "(13, \"name first individual\", \"pas_num3\", \"phone3\", "
      "\"email3\");";

  if (exec(query))
    qDebug() << "testuserOK";
  else
    qDebug() << "testuserNO";
}
