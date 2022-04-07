#include "creditmanager.h"

bool LoanManager::loan_request(IUser *user, Loan &l, LoanType type) {
  Request r(type == LoanType::CREDIT ? Request::CREDIT : Request::INSTALLMENT,
            user->get_id(), l.get_id());
  r.set_approved(send_loan(l));
  return IHistoryManager::send_request(r);
}

LoanManager::LoanManager(LoanType loan_type, IUser *user, ItemsType items_type)
    : IHistoryManager(user, items_type), loan_type(loan_type) {
  connect(USER_DB, &DataBase::updated, this, &LoanManager::update_vars);
  LoanManager::update_vars();
}

bool LoanManager::send_request(Credit *c, Request &r) {
  r.set_approved(USER_DB->update(*c));
  return IHistoryManager::send_request(r);
}

bool LoanManager::send_credit(Credit &c) { return USER_DB->add_credit(c); }

bool LoanManager::send_loan(Loan &l) { return USER_DB->add_loan(l); }

std::vector<QTableWidgetItem *> LoanManager::get_items() const {
  std::vector<QTableWidgetItem *> items;
  for (auto &c : credits)
    items.push_back(new QTableWidgetItem(c->get_info()));
  return items;
}

bool LoanManager::mark(size_t item_index, bool verify) {
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

size_t LoanManager::get_selected(size_t index) const {
  return credits[index]->get_id();
}

void LoanManager::update_vars() {
  credits.clear();
  requests.clear();

  requests = USER_DB->get_requests(Request::CREDIT, mode == ItemsType::CLIENT);
  for (auto &r : requests) {
    credits.push_back(USER_DB->get_credit(r->get_object()));
  }
}
