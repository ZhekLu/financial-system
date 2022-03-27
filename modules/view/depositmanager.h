#ifndef DEPOSITMANAGER_H
#define DEPOSITMANAGER_H

#include "modules/database/userdb.h"
#include "modules/entities/entity.h"
#include "modules/entities/individual.h"

#include <QMainWindow>
#include <QSqlQuery>
#include <QTableWidgetItem>

namespace Ui {
class DepositManager;
}

class DepositManager : public QMainWindow {
  Q_OBJECT

public:
  enum Mode { Company, Person };
  DepositManager(IUser *owner, Mode mode, QWidget *parent = nullptr);
  ~DepositManager();

private:
  Ui::DepositManager *ui;
  IUser *user;

  Mode access_rights;

  void init_grid();
  QTableWidgetItem *get_item(QString account_id, QString bank, QString balance);
};

#endif // DEPOSITMANAGER_H
