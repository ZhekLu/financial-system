#ifndef DEPOSITMANAGER_H
#define DEPOSITMANAGER_H

#include <QMainWindow>

//#include"financialsystem.h"
//#include "modules/database/userdb.h"
#include "modules/entities/individual.h"
#include <QSqlQuery>

namespace Ui {
class DepositManager;
}

class DepositManager : public QMainWindow {
  Q_OBJECT

public:
  DepositManager(Individual *owner, QWidget *parent = nullptr);
  ~DepositManager();

private:
  Ui::DepositManager *ui;
  Individual *user;

  void init();
};

#endif // DEPOSITMANAGER_H
