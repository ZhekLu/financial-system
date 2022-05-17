#include "accountaddmanager.h"

// Static

bool AccountAddManager::deposit_request(size_t initiator_id, size_t account_id,
                                        size_t period_in_month) {
  auto account = AccountManager::get_account(account_id);
  if (!account || !account->is_available())
    return false;

  auto bank = USER_DB->get_bank(account->get_bank());

  AccountAdd deposit(initiator_id, account->get_bank(), account_id,
                     QDate::currentDate(), period_in_month,
                     account->get_balance(), bank->get_percent());
  Request r(Request::DEPOSIT, initiator_id, deposit.get_id());

  return send_add(deposit, r);
}

bool AccountAddManager::salary_request(size_t initiator_id, size_t bank_id,
                                       size_t account_id,
                                       size_t period_in_month,
                                       size_t salary_amount) {
  auto account = AccountManager::get_account(account_id);
  if (!account || !account->is_available())
    return false;

  AccountAdd deposit(initiator_id, bank_id, account_id, QDate::currentDate(),
                     period_in_month, salary_amount);
  Request r(Request::SALARY, initiator_id, deposit.get_id());

  return send_add(deposit, r);
}

bool AccountAddManager::send_add(AccountAdd &a) { return USER_DB->add_add(a); }

bool AccountAddManager::send_add(AccountAdd &a, Request &r) {
  r.set_approved(send_add(a));
  return IHistoryManager::send_request(r);
}

// Object

AccountAddManager::AccountAddManager(IUser *user, AccountAdd::Type add_type)
    : IHistoryManager(user, ItemsType::SYSTEM), add_type(add_type) {
  init();
  AccountAddManager::update_vars();
}

std::vector<QTableWidgetItem *> AccountAddManager::get_items() const {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < requests.size(); i++) {
    QString item = requests[i]->get_info() + adds[i]->get_info();
    items.push_back(new QTableWidgetItem(item));
  }
  return items;
}

bool AccountAddManager::mark(size_t item_index, bool verify) {
  AccountAdd *current = adds[item_index].get();
  std::unique_ptr<Request> r = std::move(requests[item_index]);
  Request ur(verify ? Request::VERIFY : Request::UNDO, user->get_id(),
             r->get_id());

  current->set_approved(verify);
  ur.set_approved(USER_DB->update(*current));

  r->set_viewed(IHistoryManager::send_request(ur));
  if (r->is_viewed())
    qDebug() << "Request : " << USER_DB->update(*r);
  return r->is_viewed();
}

size_t AccountAddManager::get_selected(size_t index) const {
  return adds[index]->get_id();
}

void AccountAddManager::update_vars() {
  update_requests();

  // update adds;
  adds.clear();
  for (auto &r : requests) {
    adds.push_back(USER_DB->get_add(r->get_object()));
  }
}

void AccountAddManager::update_requests() {
  requests.clear();
  requests = USER_DB->get_requests(request_type, false);
}

void AccountAddManager::init() {
  request_type =
      add_type == AccountAdd::DEPOSIT ? Request::DEPOSIT : Request::SALARY;
  connect(USER_DB, &DataBase::updated, this, &AccountAddManager::update_vars);
}
