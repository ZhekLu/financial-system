#ifndef MANAGERFACTORY_H
#define MANAGERFACTORY_H

#include "depositmanager.h"
#include "modules/modes.h"

class ManagerFactory {
public:
  static QMainWindow *get_manager_widget(size_t id, LoginMode mode,
                                         QWidget *parent = nullptr) {
    IUser *current;
    if (mode == ENTITY)
      current = USER_DB->get_company(id);
    else
      current = USER_DB->get_user(id);

    if (!current)
      return nullptr;

    switch (mode) {
    case ADMIN:
    case MANAGER:
    case OPERATOR:
    case INDIVIDUAL:
    case ENTITY:
      return new DepositManager(current,
                                mode == ENTITY ? DepositManager::Company
                                               : DepositManager::Person,
                                parent);
    }
  }
};

#endif // MANAGERFACTORY_H
