#include "creditmanager.h"

bool CreditManager::credit_request(IUser *user, Bank *bank, size_t amount,
                                   size_t period_in_months) {
  Credit c(user->get_id(), amount, bank->get_percent(), period_in_months);
  Request r(Request::CREDIT, user->get_id(), c.get_id());
  r.set_approved(send_credit(c));
  return IHistoryManager::send_request(r);
}

bool CreditManager::credit_request(IUser *user, Credit &c) {
  Request r(Request::CREDIT, user->get_id(), c.get_id());
  r.set_approved(send_credit(c));
  return IHistoryManager::send_request(r);
}

CreditManager::CreditManager(IUser *user, bool viewed)
    : IHistoryManager(user), viewed_mode(viewed) {
  connect(USER_DB, &DataBase::updated, this, &CreditManager::update_vars);
  CreditManager::update_vars();
}

bool CreditManager::send_request(Credit *c, Request &r) {
  r.set_approved(USER_DB->update(*c));
  return IHistoryManager::send_request(r);
}

bool CreditManager::send_credit(Credit &c) { return USER_DB->add_credit(c); }

std::vector<QTableWidgetItem *> CreditManager::get_items() {
  std::vector<QTableWidgetItem *> items;
  for (auto &c : credits)
    items.push_back(new QTableWidgetItem(c->get_info()));
  return items;
}

bool CreditManager::mark(size_t item_index, bool verify) {
  Credit *current = credits[item_index].get();
  std::unique_ptr<Request> r = std::move(requests[item_index]);
  Request ur(verify ? Request::VERIFY : Request::UNDO, user->get_id(),
             r->get_id());
  current->set_open(verify);
  r->set_viewed(USER_DB->update(*current));
  if (r->is_viewed())
    qDebug() << "Request : " << USER_DB->update(*r);
  return r->is_viewed();
}

void CreditManager::update_vars() {
  credits.clear();
  requests.clear();

  requests = USER_DB->get_requests(Request::CREDIT, viewed_mode);
  for (auto &r : requests) {
    credits.push_back(USER_DB->get_credit(r->get_object()));
  }
}
