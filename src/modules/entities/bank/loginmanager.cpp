#include "loginmanager.h"

// Static

bool LoginManager::login_request(IUser *user, SystemUser *login_info) {
  login_info->set_user(user->get_id());
  Request r(Request::LOGIN_USER, user->get_id(), login_info->get_id());
  r.set_approved(send_login(*login_info));
  if (r.is_approved())
    r.set_approved(send_user(user, login_info->get_role()));
  return IHistoryManager::send_request(r);
}

size_t LoginManager::get_user(QString &login, QString &password,
                              LoginMode mode) {
  auto user_login = USER_DB->get_login(login);

  if (check_login(user_login.get(), password, mode))
    return user_login->get_user();

  return 0;
}

bool LoginManager::check_login(SystemUser *ul, QString &password,
                               LoginMode mode) {
  if (!ul || !ul->is_available())
    return false;
  return ul->get_role() == mode && ul->check_password(password);
}

bool LoginManager::send_login(SystemUser &l) { return USER_DB->add_login(l); }

bool LoginManager::send_user(IUser *user, LoginMode role) {
  if (role == LoginMode::ENTITY)
    return USER_DB->add_company(*(Entity *)user);
  return USER_DB->add_user(*(Individual *)user);
}

// Object

LoginManager::LoginManager(IUser *user)
    : IHistoryManager(user, ItemsType::SYSTEM) {
  connect(USER_DB, &DataBase::updated, this, &LoginManager::update_vars);
  // upd
  LoginManager::update_vars();
}

bool LoginManager::mark(size_t item_index, bool verify) {
  // TODO
}

size_t LoginManager::get_selected(size_t index) const {
  return logins[index]->get_id();
}

std::vector<QTableWidgetItem *> LoginManager::get_items() const {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {
    QString item =
        requests[i]->get_info() + logins[i]->get_info() + users[i]->get_info();
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

void LoginManager::update_vars() {
  // clear
  requests.clear();
  logins.clear();
  users.clear();

  // fill
  requests = USER_DB->get_requests(Request::LOGIN_USER, false);
  for (auto &r : requests) {
    logins.push_back(USER_DB->get_login(r->get_object()));
    users.push_back(
        std::unique_ptr<IUser>(USER_DB->get_user(logins.back()->get_user())));
  }
}
