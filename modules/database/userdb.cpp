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
                     "balance INTEGER"
                     ");");
  }

  if (!db.tables().contains("companies") && res) {
    res = query.exec("CREATE TABLE companies"
                     "("
                     "name TEXT, "
                     "type INTEGER, "
                     "PAC INTEGER, "
                     "BIC INTEGER PRIMARY KEY, "
                     "adress TEXT"
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
    qDebug() << "OK";
  else
    qDebug() << "NO";
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
      "(13, \"name first individual\", \"pas_num3\", \"phone3\", \"email3\");";

  if (exec(query))
    qDebug() << "testuserOK";
  else
    qDebug() << "testuserNO";
}

// User

Individual *UserDB::get_user(size_t id) {
  QString query = tr("SELECT full_name, pass_number, "
                     "pass_id, phone, email "
                     "FROM users WHERE id = ") +
                  qs(QString::number(id));
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
  QString query = tr("DELETE FROM users WHERE login = ") + qs(login);
  exec(query);
  emit DataBase::updated();
}

// void UserDB::add_account(BankAccount acc) {
//   QString query =
//       tr("INSERT INTO accounts(id, user_id, bank_id, balance) VALUES (") +
//       //      QString::number(acc.get_id()) + "," +
//       QString::number(acc.get_owner_id()) + "," +
//       QString::number(acc.get_bank_id()) + "," +
//       QString::number(acc.get_balance()) + ");";
//   exec(query);
//   emit DataBase::updated();
// }

// void UserDB::add_company(Company company) {
//   QString query =
//       tr("INSERT INTO companies(id, name, type, PAC, BIC, adress) VALUES
//       (")
//       +
//       //      QString::number(company.get_id()) + "," +
//       QString::fromStdString(company._name) + "," +
//       QString::number(company._type) + "," +
//       QString::fromStdString(company._PAC) + "," +
//       QString::fromStdString(company._BIC) + "," +
//       QString::fromStdString(company._adress) + ");";
//   exec(query);
//   emit DataBase::updated();
// }

// void UserDB::remove_company(size_t id) {
//   QString query = tr("DELETE FROM companies WHERE id = ") +
//   QString::number(id); exec(query); emit DataBase::updated();
// }
