#include "addcardwidget.h"
#include "ui_addcardwidget.h"

AddCardWidget::AddCardWidget(
    IUser *user, std::unordered_map<size_t, std::unique_ptr<Bank>> &banks_list,
    QWidget *parent)
    : QWidget(parent), ui(new Ui::AddCardWidget), user(user),
      banks(banks_list) {
  ui->setupUi(this);
  mode = Mode::CardAdder;
  update_combobox();
}

AddCardWidget::AddCardWidget(
    std::unordered_map<size_t, std::unique_ptr<Bank>> &banks_list,
    QWidget *parent)
    : QWidget(parent), ui(new Ui::AddCardWidget), user(nullptr),
      banks(banks_list) {
  ui->setupUi(this);
  mode = Mode::BankSelector;
  ui->confirm_but->setText("Continue");
  update_combobox();
}

AddCardWidget::~AddCardWidget() { delete ui; }

void AddCardWidget::show() { update_combobox(); }

void AddCardWidget::on_cance_but_clicked() { emit closed(); }

void AddCardWidget::on_confirm_but_clicked() {
  if (mode == CardAdder) {
    send_add_account(
        user->get_id(),
        banks[banks_indexes[ui->bank_chooser->currentIndex()]]->get_id());
    emit closed();
  } else {
    emit selected(banks[banks_indexes[ui->bank_chooser->currentIndex()]].get());
  }
}

void AddCardWidget::send_add_account(size_t user_id, size_t bank_id) {
  std::unique_ptr<BankAccount> to_add(new BankAccount(user_id, bank_id));
  qDebug() << "Add account : "
           << AccountManager::add_account_request(to_add.get());
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
  banks_indexes.push_back(company->bank_bic);
  ui->bank_chooser->addItem(
      QString::fromStdString(banks[banks_indexes.back()]->get_name()));
}

void AddCardWidget::person_mode() {
  for (auto &bank : banks) {
    ui->bank_chooser->addItem(QString::fromStdString(bank.second->get_name()));
    banks_indexes.push_back(bank.second->get_id());
  }
}
