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
                     "id INTEGER PRIMARY KEY, "
                     "name TEXT, "
                     "type INTEGER, "
                     "PAC INTEGER, "
                     "BIC INTEGER, "
                     "adress TEXT"
                     ");");
  }

  return res;
}

bool UserDB::is_login_busy(QString login) {
  QString query =
      tr("SELECT login FROM system_users WHERE login = ") + qs(login);
  exec(query);

  return db_query->first();
}

bool UserDB::contains(SystemUser user) {
  QString query = tr("SELECT password, role FROM system_users WHERE login = ") +
                  qs(user.login);
  exec(query);
  if (!db_query->next())
    return false;

  QString password = db_query->value(0).toString();
  int role = db_query->value(1).toInt();
  return QString::fromStdString(user.password) == password && user.mode == role;
}

void UserDB::print_all_users() {
  QString query = "SELECT * FROM system_users";
  if (exec(query))
    qDebug() << "OK";
  else
    qDebug() << "NO";
  qDebug() << "ALL:\n";
  while (db_query->next()) {
    qDebug() << "Start:" << db_query->value(0).toString()
             << db_query->value(1).toString() << db_query->value(2).toString()
             << ":End";
  }
  qDebug() << ":ALL\n";
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

void UserDB::test_login() {

  QString query = ".tables";
  if (!exec(query))
    qDebug() << ".tables(no)";
  while (db_query->next()) {
    qDebug() << db_query->value(0).toString();
  }

  query = "INSERT INTO system_users (login, password, role, user_id)"
          "VALUES (\"login1\", \"pass1\", 1, 1),"
          "(\"login2\", \"pass2\", 2, 2);";
  if (exec(query))
    qDebug() << "testOK";
  else
    qDebug() << "testNO";
}

// User *UserDB::get_user(QString login) {
//   QString query =
//       tr("SELECT user_id FROM system_users WHERE login = ") + qs(login);
//   exec(query);

//  if (!db_query->next())
//    return nullptr;

//  int id = db_query->value(0).toInt();

//  query = tr("SELECT full_name, pass_number, pass_id, phone, email "
//             "FROM users WHERE id = ") +
//          qs(QString::number(id));
//  exec(query);

//  std::string name = db_query->value(0).toString().toStdString();
//  std::string p_number = db_query->value(1).toString().toStdString();
//  std::string p_id = db_query->value(2).toString().toStdString();
//  std::string phone = db_query->value(3).toString().toStdString();
//  std::string email = db_query->value(4).toString().toStdString();

//  return new User(id, name, p_number, p_id, phone, email);
//}

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
