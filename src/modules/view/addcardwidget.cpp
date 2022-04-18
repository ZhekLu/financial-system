#include "addcardwidget.h"
#include "ui_addcardwidget.h"

AddCardWidget::AddCardWidget(Mode mode, IUser *user, QWidget *parent)
    : QWidget(parent), ui(new Ui::AddCardWidget), user(user), mode(mode) {
  ui->setupUi(this);
  update_combobox();
}

AddCardWidget::AddCardWidget(IUser *user, QWidget *parent)
    : AddCardWidget(Mode::CardAdder, user, parent) {}

AddCardWidget::AddCardWidget(QWidget *parent)
    : AddCardWidget(Mode::BankSelector, nullptr, parent) {
  ui->confirm_but->setText("Continue");
}

AddCardWidget::~AddCardWidget() { delete ui; }

void AddCardWidget::show() { update_combobox(); }

void AddCardWidget::update() {
  banks = USER_DB->get_hash_banks();
  update_combobox();
}

void AddCardWidget::on_cance_but_clicked() { emit closed(); }

void AddCardWidget::on_confirm_but_clicked() {
  if (mode == CardAdder) {
    send_add_account(
        user->get_id(),
        banks[banks_indexes[ui->bank_chooser->currentIndex()]]->get_id());
    emit closed();
    return;
  }

  emit selected(banks[banks_indexes[ui->bank_chooser->currentIndex()]].get());
}

void AddCardWidget::send_add_account(size_t user_id, size_t bank_id) {
  qDebug() << "Add account : "
           << AccountStateManager::add_account_request(user_id, bank_id);
}

void AddCardWidget::update_combobox() {
  // clear
  ui->bank_chooser->clear();
  banks_indexes.clear();

  // fill
  if (user && user->get_role() == LoginMode::ENTITY)
    company_mode();
  else
    person_mode();
}

void AddCardWidget::company_mode() {
  Entity *company = (Entity *)user;
  banks_indexes.push_back(company->get_bank());
  ui->bank_chooser->addItem(banks[banks_indexes.back()]->get_name());
}

void AddCardWidget::person_mode() {
  for (auto &bank : banks) {
    ui->bank_chooser->addItem(bank.second->get_name());
    banks_indexes.push_back(bank.second->get_id());
  }
}
