#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QtSql>

class DataBase : public QObject {
  Q_OBJECT
public:
  explicit DataBase(QString &databasename, QObject *parent = 0);
  ~DataBase();
signals:
  void updated();

protected:
  QSqlDatabase db;
  QSqlQuery *db_query;
  QString db_name;

  bool exec(QString);
  QString qs(QString);
  QString qs(std::string);

  virtual bool restoreDataBase();
  virtual bool openDataBase();
  virtual void closeDataBase();
  virtual bool connectToDataBase();
  virtual bool createTables() = 0;
};

#endif // DATABASE_H
