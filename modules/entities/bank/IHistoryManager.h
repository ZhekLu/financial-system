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
  IHistoryManager() {
    connect(USER_DB, &DataBase::updated, this, &IHistoryManager::update);
  }

  virtual std::vector<QTableWidgetItem *> get_items() = 0;
  virtual bool undo(size_t item_index) = 0;

private slots:
  virtual void update() = 0;
};

#endif // IHISTORYMANAGER_H
