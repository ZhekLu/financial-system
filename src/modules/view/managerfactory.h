#ifndef MANAGERFACTORY_H
#define MANAGERFACTORY_H

#include "clientwindow.h"
#include "entityclientwindow.h"
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
      return new ClientWindow(current, ClientWindow::AccessMode::Person,
                              parent);
    case ENTITY:
      return new EntityClientWindow((Entity *)current, parent);
    }
    return nullptr;
  }
  // todo! refactor this mehtod
};

#endif // MANAGERFACTORY_H
