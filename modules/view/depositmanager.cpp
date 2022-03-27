#include "depositmanager.h"
#include "ui_depositmanager.h"

DepositManager::DepositManager(IUser *owner, Mode mode, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DepositManager), user(owner),
      access_rights(mode), current_account(nullptr) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  update();
  connect(USER_DB, &DataBase::updated, this, &DepositManager::update);
}

DepositManager::~DepositManager() {
  delete ui;
  delete user;
}

void DepositManager::update() {
  // clear
  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setColumnCount(1);
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

QTableWidgetItem *DepositManager::get_item(BankAccount *acc, QString bank) {
  QString item = acc->get_info() + "Bank : " + bank;
  return new QTableWidgetItem(item);
}

void DepositManager::on_log_out_but_clicked() { this->close(); }

void DepositManager::on_info_but_clicked() {
  QMessageBox::about(this, "your info", user->get_info());
}

void DepositManager::on_freeze_but_clicked() {
  if (AccountManager::freeze_request(current_account))
    QMessageBox::information(this, ui->freeze_but->text(), "Success");
}

void DepositManager::on_withdraw_but_clicked() {}

void DepositManager::on_new_card_but_clicked() {}

void DepositManager::on_transfer_but_clicked() {}

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
