#ifndef IMANAGER_H
#define IMANAGER_H

#include "modules/database/userdb.h"
#include <QObject>
#include <QTableWidgetItem>
#include <memory>
#include <vector>

class IManager : public QObject {
  Q_OBJECT

public:
  enum ItemsType { CLIENT, SYSTEM, NON };

  IManager(IUser *sender, ItemsType type) : user(sender), mode(type) {}
  IManager(IUser *sender) : user(sender), mode(NON) {}

  virtual std::vector<QTableWidgetItem *> get_items() const = 0;
  virtual size_t get_selected(size_t index) const = 0;

signals:
  void updated();

protected:
  IUser *user;
  ItemsType mode;

private slots:
  virtual void update_vars() = 0;
};

#endif // IMANAGER_H
