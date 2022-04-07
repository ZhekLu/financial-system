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
  enum ItemsType { CLIENT, SYSTEM, NON };

  IHistoryManager(IUser *sender, ItemsType type) : user(sender), mode(type) {}
  IHistoryManager(IUser *sender) : user(sender), mode(NON) {}

  virtual std::vector<QTableWidgetItem *> get_items() const = 0;
  virtual bool mark(size_t item_index, bool verify) = 0;
  virtual size_t get_selected(size_t index) const = 0;

signals:
  void updated();

protected:
  IUser *user;
  ItemsType mode;

  static bool send_request(Request &r) {
    USER_DB->add_request(r);
    return r.is_approved();
  }

private slots:
  virtual void update_vars() = 0;
};

#endif // IHISTORYMANAGER_H
