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
                     "receiver INTEGER,"
                     "approved BOOLEAN"
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

size_t UserDB::get_id_by_login(SystemUser user) {
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
                  qs(QString::number(user.id)) + ");";
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
  QString query = ("SELECT name, type, PAC, BIC, adress, bank_bic"
                   "FROM companies WHERE id = ") +
                  QString::number(id);
  exec(query);

  if (!db_query->next())
    return nullptr;

  std::string name = db_query->value(0).toString().toStdString();
  int type = db_query->value(1).toInt();
  size_t PAC = db_query->value(2).toULongLong();
  size_t BIC = db_query->value(3).toULongLong();
  std::string adress = db_query->value(4).toString().toStdString();
  size_t bank_bic = db_query->value(5).toULongLong();

  return new Entity(id, Entity::Type(type), name, PAC, BIC, adress, bank_bic);
}

// Bank accounts

void UserDB::add_account(BankAccount acc) {
  QString query = ("INSERT INTO accounts(id, user_id, bank_id, balance, "
                   "frozen) VALUES (") +
                  QString::number(acc.get_id()) + "," +
                  QString::number(acc.get_owner_id()) + "," +
                  QString::number(acc.get_bank_id()) + "," +
                  QString::number(acc.get_balance()) + "," +
                  QString::number(acc.get_frozen()) + ");";
  if (exec(query))
    emit DataBase::updated();
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
      qs(QString::number(id));
  exec(query);

  if (!db_query->next())
    return nullptr;

  size_t user_id = db_query->value(0).toInt();
  size_t bank_id = db_query->value(1).toInt();
  size_t balance = db_query->value(2).toInt();
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

std::vector<BankAccount *> UserDB::get_user_accounts(size_t user_id) {
  QString query =
      ("SELECT id, bank_id, balance FROM accounts WHERE user_id = ") +
      qs(QString::number(user_id));
  exec(query);

  std::vector<BankAccount *> accounts;

  while (db_query->next()) {
    size_t id = db_query->value(0).toInt();
    size_t bank_id = db_query->value(1).toInt();
    size_t balance = db_query->value(2).toInt();
    bool frozen = db_query->value(3).toBool();
    accounts.push_back(new BankAccount(id, user_id, bank_id, balance, frozen));
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
                          "(id, type, sender, receiver, approved)"
                          " VALUES %1;")
                      .arg(r.get_values_query());
  if (exec(query))
    emit DataBase::updated();
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
