#ifndef DEPOSITMANAGER_H
#define DEPOSITMANAGER_H

#include <QMainWindow>

//#include"financialsystem.h"
#include "modules/entities/user.h"

namespace Ui {
class DepositManager;
}

class DepositManager : public QMainWindow {
  Q_OBJECT

public:
  DepositManager(User *owner, QWidget *parent = nullptr);
  ~DepositManager();

private:
  Ui::DepositManager *ui;
  User *user;

  void init();
};

#endif // DEPOSITMANAGER_H
