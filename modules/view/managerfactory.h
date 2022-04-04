#ifndef MANAGERFACTORY_H
#define MANAGERFACTORY_H

#include "clientwindow.h"
#include "depositmanager.h"
#include "modules/modes.h"
#include "systemwindow.h"

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
      return new SystemWindow(current, parent);
    case INDIVIDUAL:
    case ENTITY:
      return new ClientWindow(current,
                              mode == ENTITY ? ClientWindow::AccessMode::Company
                                             : ClientWindow::AccessMode::Person,
                              parent);
    }
  }
};

#endif // MANAGERFACTORY_H
