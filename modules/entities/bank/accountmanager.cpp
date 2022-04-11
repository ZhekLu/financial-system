#include "accountmanager.h"

// Static

bool AccountManager::add_account_request(size_t sender_id, BankAccount *acc) {
  Request r(Request::LOGIN_ACCOUNT, sender_id, acc->get_id());
  r.set_approved(USER_DB->add_account(acc));
  return IHistoryManager::send_request(r);
}

// Object

AccountManager::AccountManager(IUser *user, ItemsType) : IHistoryManager(user) {
  connect(USER_DB, &DataBase::updated, this, &AccountManager::update_vars);
  AccountManager::update_vars();
}

std::vector<QTableWidgetItem *> AccountManager::get_items() const {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {
    QString item = requests[i]->get_info() + accounts[i]->get_info();
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

bool AccountManager::mark(size_t item_index, bool verify) {
  BankAccount *current = accounts[item_index].get();
  std::unique_ptr<Request> r = std::move(requests[item_index]);
  current->set_available(verify);

  Request ur(verify ? Request::VERIFY : Request::UNDO, user->get_id(),
             r->get_id());
  ur.set_approved(USER_DB->update(*current));

  r->set_viewed(IHistoryManager::send_request(ur));
  if (r->is_viewed())
    qDebug() << "Request : " << USER_DB->update(*r);
  return r->is_viewed();
}

size_t AccountManager::get_selected(size_t index) const {
  return accounts[index]->get_id();
}

void AccountManager::update_vars() {
  requests.clear();
  accounts.clear();

  requests = USER_DB->get_requests(Request::Type::LOGIN_ACCOUNT, false);
  for (auto &r : requests) {
    accounts.push_back(USER_DB->get_account(r->get_object()));
  }
}

bool AccountManager::approve_login(BankAccount *acc) {
  Request r(Request::VERIFY, user->get_id(), acc->get_id());
  acc->set_available(true);

  r.set_approved(USER_DB->update(*acc));
  return IHistoryManager::send_request(r);
}
