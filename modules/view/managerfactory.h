#ifndef MANAGERFACTORY_H
#define MANAGERFACTORY_H

#include "depositmanager.h"
#include "modules/modes.h"

class ManagerFactory {
public:
  ManagerFactory(Individual *u) {
    dm = new DepositManager(u);
    dm->show();
  }

  static QMainWindow *get_manager_widget(Individual *u, QWidget *parent = nullptr) {
    return new DepositManager(u, parent);
  }

private:
  DepositManager *dm;
};

#endif // MANAGERFACTORY_H
