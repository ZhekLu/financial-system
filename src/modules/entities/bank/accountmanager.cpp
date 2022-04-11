#include "accountmanager.h"

// Static

bool AccountManager::freeze_request(size_t sender_id, size_t account_id,
                                    bool freeze) {
  std::unique_ptr<BankAccount> acc(USER_DB->get_account(account_id));
  if (!acc)
    return false;
  if (acc->is_frozen() == freeze)
    return true;
  acc->revert_frozen();

  Request r(Request::FREEZE, sender_id, acc->get_id());
  r.set_approved(USER_DB->update(*acc));
  return IHistoryManager::send_request(r);
}

bool AccountManager::block_request(size_t sender_id, size_t account_id,
                                   bool block) {
  std::unique_ptr<BankAccount> acc(USER_DB->get_account(account_id));
  if (!acc)
    return false;
  if (acc->is_blocked() == block)
    return true;
  Request::Type type = block ? Request::BLOCK : Request::UNBLOCK;
  Request r(type, sender_id, acc->get_id());
  IHistoryManager::send_request(r);
  return true;
}

bool AccountManager::add_account_request(size_t sender_id, BankAccount *acc) {
  Request r(Request::LOGIN_ACCOUNT, sender_id, acc->get_id());
  r.set_approved(USER_DB->add_account(acc));
  return IHistoryManager::send_request(r);
}

// Object

AccountManager::AccountManager(IUser *user, Mode mode, ItemsType)
    : IHistoryManager(user), mode(mode) {
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

  switch (mode) {
  case AccountManager::LOGIN:
    current->set_available(verify);
    break;
  case AccountManager::BLOCK:
    bool blocked = r->get_type() == Request::BLOCK;
    if (verify)
      current->set_blocked(blocked);
    break;
  }

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

  requests = mode == Mode::LOGIN
                 ? USER_DB->get_requests(Request::Type::LOGIN_ACCOUNT, false)
                 : USER_DB->get_block_requests(false);
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