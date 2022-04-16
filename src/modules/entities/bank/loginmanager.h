#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include "IHistoryManager.h"

class LoginManager : public IHistoryManager {
public:
  static bool login_request(IUser *user, SystemUser *login_info);
  static size_t get_user(QString &login, QString &password, LoginMode mode);

  // Manager
  LoginManager(IUser *user);

  bool mark(size_t item_index, bool verify) override;
  size_t get_selected(size_t index) const override;
  std::vector<QTableWidgetItem *> get_items() const override;

private slots:
  void update_vars() override;

private:
  // fields
  std::vector<std::unique_ptr<SystemUser>> logins;
  std::vector<std::unique_ptr<IUser>> users;
  std::vector<std::unique_ptr<Request>> requests;

  // static
  static bool send_login(SystemUser &);
  static bool send_user(IUser *user, LoginMode role);
  static bool check_login(SystemUser *, QString &password, LoginMode mode);
};

#endif // LOGINMANAGER_H
