#include "database.h"

DataBase::DataBase(QString &databasename, QObject *parent)
    : QObject(parent), db_name(databasename) {}

DataBase::~DataBase() { delete db_query; }

bool DataBase::exec(QString str) { return db_query->exec(str); }

QString DataBase::qs(QString str) { return "'" + str + "'"; }

QString DataBase::qs(std::string str) {
  return qs(QString::fromStdString(str));
}

bool DataBase::connectToDataBase() {
  /*
   * Перед подключением к базе данных производим проверку на её существование.
   * В зависимости от результата производим открытие базы данных или её
   * восстановление
   */
  bool res = QFile(db_name).exists() ? openDataBase() : restoreDataBase();
  if (res)
    db_query = new QSqlQuery();
  else
    qDebug() << "Everything went wrong;";
  return res;
}

bool DataBase::restoreDataBase() {
  if (openDataBase()) {
    return createTables();
  }
  qDebug() << "RestoreDatabase: fault;";
  return false;
}

bool DataBase::openDataBase() {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(db_name);
  return db.open();
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase() { db.close(); }
