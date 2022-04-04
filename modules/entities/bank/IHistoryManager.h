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
  IHistoryManager(IUser *sender) : user(sender) {}

  virtual std::vector<QTableWidgetItem *> get_items() = 0;
  virtual bool mark(size_t item_index, bool verify) = 0;

signals:
  void updated();

protected:
  IUser *user;

  static bool send_request(Request &r) {
    USER_DB->add_request(r);
    return r.is_approved();
  }

private slots:
  virtual void update_vars() = 0;
};

#endif // IHISTORYMANAGER_H
