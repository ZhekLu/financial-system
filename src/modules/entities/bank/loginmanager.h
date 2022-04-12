#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "IHistoryManager.h"

class LoginManager : public IHistoryManager {
public:
  static bool login_request(IUser *user, SystemUser *login_info);

  // Manager
  LoginManager(IUser *user);

  bool mark(size_t item_index, bool verify) override;
  size_t get_selected(size_t index) const override;
  std::vector<QTableWidgetItem *> get_items() const override;

private slots:
  void update_vars() override;

private:
  // fields
  Request::Type request_type;
  std::vector<std::unique_ptr<SystemUser>> logins;
  std::vector<std::unique_ptr<Request>> requests;
};

#endif // LOGINMANAGER_H
