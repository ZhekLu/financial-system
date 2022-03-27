#include "depositmanager.h"
#include "ui_depositmanager.h"

DepositManager::DepositManager(IUser *owner, Mode mode, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DepositManager), user(owner),
      access_rights(mode), current_account(nullptr) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  update();
  connect(USER_DB, &DataBase::updated, this, &DepositManager::update);
  init();
}

DepositManager::~DepositManager() { delete ui; }

void DepositManager::update() {
  // clear
  switch_widget(true, false);
  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setColumnCount(1);
  current_account = nullptr;
  ui->tableWidget->clearSelection();
  accounts.clear();

  update_grid();

  // style settings
  ui->tableWidget->resizeColumnsToContents();
  ui->tableWidget->resizeRowsToContents();
  ui->tableWidget->setSelectionMode(
      QAbstractItemView::SingleSelection); // only one selection
  ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void DepositManager::update_grid() {
  // fill with data
  QSqlQuery query;
  query.exec(QString("select id, bank_id, balance, frozen, name "
                     "from accounts inner join companies "
                     "on accounts.bank_id=companies.BIC where user_id=%1 ;")
                 .arg(QString::number(user->id)));
  for (int i = 0; query.next(); i++) {

    size_t id = query.value(0).toULongLong();
    size_t bank_id = query.value(1).toULongLong();
    size_t balance = query.value(2).toULongLong();
    bool frozen = query.value(3).toBool();
    QString bank_name = query.value(4).toString();
    accounts.push_back(
        std::make_unique<BankAccount>(id, user->id, bank_id, balance, frozen));

    QTableWidgetItem *item = get_item(accounts.back().get(), bank_name);

    ui->tableWidget->insertRow(i);
    ui->tableWidget->setItem(i, 0, item);
  }
}

void DepositManager::init() {

  ui->transfer_widget->hide();
  card_validator = std::make_unique<QRegularExpressionValidator>(
      QRegularExpression("^[0-9]{9}$"));
  ui->id_line->setValidator(card_validator.get());

  amount_validator = std::make_unique<QIntValidator>(0, 0);
  ui->amount_line->setValidator(amount_validator.get());
}

QTableWidgetItem *DepositManager::get_item(BankAccount *acc, QString bank) {
  QString item = acc->get_info() + "Bank : " + bank;
  return new QTableWidgetItem(item);
}

void DepositManager::switch_widget(bool to_table, bool to_transfer) {
  ui->tableWidget->setVisible(to_table);
  ui->new_card_but->setVisible(to_table);
  ui->transfer_but->setVisible(to_table);
  ui->freeze_but->setVisible(to_table);
  ui->withdraw_but->setVisible(to_table);
  ui->log_out_but->setVisible(to_table);
  ui->info_but->setVisible(to_table);
  ui->transfer_widget->setVisible(!to_table);
  ui->id_label->setVisible(to_transfer);
  ui->id_line->setVisible(to_transfer);
}

void DepositManager::clean_transfer_widget() {
  ui->id_line->clear();
  ui->amount_line->clear();
}

void DepositManager::on_log_out_but_clicked() { this->close(); }

void DepositManager::on_info_but_clicked() {
  QMessageBox::about(this, "your info", user->get_info());
}

void DepositManager::on_freeze_but_clicked() {
  if (current_account)
    qDebug() << ui->freeze_but->text()
             << AccountManager::freeze_request(current_account);
}

void DepositManager::on_withdraw_but_clicked() {
  if (!current_account)
    return;
  switch_widget(false, false);
  amount_validator->setTop(current_account->get_balance());
}

void DepositManager::on_new_card_but_clicked() {}

void DepositManager::on_transfer_but_clicked() {
  if (!current_account)
    return;
  switch_widget(false);
  amount_validator->setTop(current_account->get_balance());
}

void DepositManager::on_tableWidget_cellClicked(int row, int) {
  current_account = accounts[row].get();
  if (current_account->get_frozen()) {
    ui->freeze_but->setText("Unfreeze");
    ui->transfer_but->setEnabled(false);
    ui->withdraw_but->setEnabled(false);
  } else {
    ui->freeze_but->setText("Freeze");
    ui->transfer_but->setEnabled(true);
    ui->withdraw_but->setEnabled(true);
  }
}

void DepositManager::on_confirm_but_clicked() {

  // check if data is valid

  if (ui->id_label->isVisible()) {
    QString card = ui->id_line->text();
    int pos = 0;
    if (card_validator->validate(card, pos) != QValidator::Acceptable) {
      QMessageBox::warning(this, "input error", "incorrect card number");
      return;
    }
  }

  size_t amount = ui->amount_line->text().toULongLong();
  if (!current_account->can_pay(amount)) {
    QMessageBox::warning(this, "input error", "u don't have enought money");
    return;
  }
  if (!amount)
    return;

  if (ui->id_label->isVisible()) {
    size_t destination = ui->id_line->text().toULongLong();
    if (destination != current_account->id)
      qDebug() << "Transaction:"
               << AccountManager::transfer_request(current_account, destination,
                                                   amount);
  } else {
    qDebug() << "Withdraw"
             << AccountManager::withdraw_request(current_account, amount);
  }
  on_cancel_but_clicked();
}

void DepositManager::on_cancel_but_clicked() {
  switch_widget(true);
  clean_transfer_widget();
}
