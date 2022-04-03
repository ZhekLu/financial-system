#include "creditmanager.h"

bool CreditManager::credit_request(IUser *user, Bank *bank, size_t amount,
                                   size_t period_in_months) {
  Credit c(user->get_id(), amount, bank->get_percent(), period_in_months);
  Request r(Request::CREDIT, user->get_id(), c.get_id());
  r.is_approved = send_credit(c);
  return send_request(r);
}

bool CreditManager::credit_request(IUser *user, Credit &c) {
  Request r(Request::CREDIT, user->get_id(), c.get_id());
  r.is_approved = send_credit(c);
  return send_request(r);
}

CreditManager::CreditManager() { CreditManager::update(); }

bool CreditManager::send_request(Request &r) {
  USER_DB->add_request(r);
  return r.is_approved;
}

bool CreditManager::send_credit(Credit &c) { return USER_DB->add_credit(c); }

std::vector<QTableWidgetItem *> CreditManager::get_items() {
  std::vector<QTableWidgetItem *> items;
  for (auto &c : credits)
    items.push_back(new QTableWidgetItem(c->get_info()));
  return items;
}

bool CreditManager::undo(size_t item_index) {}

void CreditManager::update() {
  credits.clear();
  credits = USER_DB->get_credits();
}
