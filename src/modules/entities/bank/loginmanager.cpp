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

bool LoginManager::send_login(SystemUser &l) { return USER_DB->add_login(l); }

bool LoginManager::send_user(IUser *user, LoginMode role) {
  if (role == LoginMode::ENTITY)
    return USER_DB->add_company(*(Entity *)user);
  return USER_DB->add_user(*(Individual *)user);
}

// Object

LoginManager::LoginManager(IUser *user)
    : IHistoryManager(user, ItemsType::SYSTEM) {}

bool LoginManager::mark(size_t item_index, bool verify) {}

size_t LoginManager::get_selected(size_t index) const {}

std::vector<QTableWidgetItem *> LoginManager::get_items() const {}

void LoginManager::update_vars() {}
