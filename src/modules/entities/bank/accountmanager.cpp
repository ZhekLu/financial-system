#include "accountmanager.h"

// Static

std::unique_ptr<BankAccount> AccountManager::get_account(size_t id) {
  auto acc = USER_DB->get_account(id);
  update_account(acc.get());
  return acc;
}

void AccountManager::update_account(BankAccount *acc) {
  auto adds = USER_DB->get_adds(acc->get_id(), true);

  for (auto &add : adds) {
    acc->top_up(add->pay());
    qDebug() << "Add upd : " << USER_DB->update(*add);
  }

  qDebug() << "Acc upd : " << USER_DB->update(*acc);
}

// Object

AccountManager::AccountManager(IUser *user) : IManager(user) {
  connect(USER_DB, &DataBase::updated, this, &AccountManager::update_vars);
  AccountManager::update_vars();
}

std::vector<QTableWidgetItem *> AccountManager::get_items() const {
  std::vector<QTableWidgetItem *> items;
  for (size_t i = 0; i < accounts.size(); i++) {
    QString bank_name = banks.at(accounts[i]->get_bank())->get_name();
    QTableWidgetItem *item = get_item(accounts[i].get(), bank_name);
    items.push_back(item);
  }
  return items;
}

size_t AccountManager::get_selected(size_t index) const {
  return accounts[index]->get_id();
}

BankAccount *AccountManager::get_selected_account(size_t index) const {
  return accounts[index].get();
}

void AccountManager::update_vars() {
  accounts = USER_DB->get_user_accounts(user->get_id());
  banks = USER_DB->get_hash_banks();

  for (auto &acc : accounts) {
    update_account(acc.get());
  }
}

QTableWidgetItem *AccountManager::get_item(BankAccount *acc,
                                           QString bank) const {
  QString item = acc->get_info() + "Bank : " + bank;
  return new QTableWidgetItem(item);
}
