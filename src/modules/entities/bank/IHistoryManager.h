#ifndef IHISTORYMANAGER_H
#define IHISTORYMANAGER_H

#include "IManager.h"

class IHistoryManager : public IManager {
  Q_OBJECT

public:
  IHistoryManager(IUser *sender, ItemsType type) : IManager(sender, type) {}
  IHistoryManager(IUser *sender) : IManager(sender) {}

  virtual bool mark(size_t item_index, bool verify) = 0;

protected:
  static bool send_request(Request &r) {
    USER_DB->add_request(r);
    return r.is_approved();
  }
};

#endif // IHISTORYMANAGER_H
