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

bool CreditManager::send_request(Request &r) {
  USER_DB->add_request(r);
  return r.is_approved;
}

bool CreditManager::send_credit(Credit &c) { return USER_DB->add_credit(c); }
