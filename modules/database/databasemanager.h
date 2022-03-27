#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlQuery>
#include <QString>
#include <string>

class DataBaseManager {
protected:
  // methods
  static bool exec(QString query) {
    QSqlQuery db_query;
    return db_query.exec(query);
  }
  static QString qs(QString str) { return "'" + str + "'"; }
  static QString qs(size_t num) { return QString::number(num); }
  static QString qs(std::string str) { return qs(QString::fromStdString(str)); }
  // TODO:encrypt();
};

#endif // DATABASEMANAGER_H
