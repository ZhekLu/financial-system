#include "addcardwidget.h"
#include "ui_addcardwidget.h"

AddCardWidget::AddCardWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AddCardWidget) {
  ui->setupUi(this);
}

AddCardWidget::~AddCardWidget() { delete ui; }

void AddCardWidget::on_cance_but_clicked() { emit closed(); }

void AddCardWidget::on_confirm_but_clicked() {}

void AddCardWidget::send_add_account(size_t user_id, size_t bank_id) {
  std::unique_ptr<BankAccount> to_add(new BankAccount(user_id, bank_id));
  qDebug() << AccountManager::add_account_request(to_add.get());
}

void AddCardWidget::update_combobox() {
  // clear
  ui->bank_chooser->clear();
  //  banks.clear();

  // fill
  QSqlQuery query;
  query.exec(
      "select BIC, name from system_banks left join companies using(BIC)");
  while (query.next()) {
    size_t bank_id = query.value(0).toULongLong();
    QString bank_name = query.value(1).toString();
    ui->bank_chooser->addItem(bank_name);
    //    banks.push_back(bank_id);
  }
}
