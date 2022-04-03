#ifndef IHISTORYMANAGER_H
#define IHISTORYMANAGER_H

#include "modules/database/userdb.h"
#include <QObject>
#include <QTableWidgetItem>
#include <memory>
#include <vector>

class IHistoryManager : public QObject {
  Q_OBJECT

public:
  IHistoryManager(IUser *sender) : user(sender) {
    connect(USER_DB, &DataBase::updated, this, &IHistoryManager::update);
  }

  virtual std::vector<QTableWidgetItem *> get_items() = 0;
  virtual bool undo(size_t item_index) = 0;
  virtual bool verify(size_t) { return true; }

signals:
  void updated();

protected:
  IUser *user;

private slots:
  virtual void update() = 0;
};

#endif // IHISTORYMANAGER_H
