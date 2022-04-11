#ifndef IDGENERATOR_H
#define IDGENERATOR_H

#include <QDebug>
#include <QRandomGenerator>
#include <QSqlQuery>

class IdGenerator {
public:
  IdGenerator(int len, QString table_name, QString col_name)
      : table(table_name), column(col_name), generator(rand()) {
    limiter = 1;
    for (int i = 0; i < len; i++)
      limiter *= 10;
  }

  size_t GenerateId() {
    generator.seed(rand());
    size_t id;
    do {
      id = generator.generate64() % limiter;
    } while (is_id_busy(id));
    return id;
  }

private:
  bool is_id_busy(size_t id) {
    QSqlQuery query;
    query.exec(QString("select * from %1 where %2 = %3")
                   .arg(table, column, QString::number(id)));
    return query.next();
  }

  size_t limiter;
  QString table;
  QString column;
  QRandomGenerator generator;
};

#endif // IDGENERATOR_H
