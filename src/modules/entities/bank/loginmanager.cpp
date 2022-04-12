#include "loginmanager.h"

// Static

bool LoginManager::login_request(IUser *user, SystemUser *login_info) {
  //  Request r(Request::LOGIN_USER, user->get_id(), l.get_id());
  //  r.set_approved(send_loan(l));
  //  return IHistoryManager::send_request(r);
}

// Object

LoginManager::LoginManager(IUser *user)
    : IHistoryManager(user, ItemsType::SYSTEM) {}

bool LoginManager::mark(size_t item_index, bool verify) {}

size_t LoginManager::get_selected(size_t index) const {}

std::vector<QTableWidgetItem *> LoginManager::get_items() const {}

void LoginManager::update_vars() {}
