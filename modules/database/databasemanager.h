#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlQuery>
#include <QString>
#include <string>

class DataBaseManager {
protected:
  // methods
  static bool exec(QString query) { return db_query->exec(query); }
  static QString qs(QString str) { return "'" + str + "'"; }
  static QString qs(size_t num) { return QString::number(num); }
  static QString qs(std::string str) { return qs(QString::fromStdString(str)); }
  // TODO:encrypt();

  // fields
  static inline QSqlQuery *db_query = new QSqlQuery();
};

#endif // DATABASEMANAGER_H
