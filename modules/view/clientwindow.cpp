#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(IUser *owner, AccessMode mode, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClientWindow), user(owner), mode(mode),
      current_account(nullptr) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  init();
  update();
}

ClientWindow::~ClientWindow() { delete ui; }

void ClientWindow::update() {
  // clear
  ui->stacked_widget->setCurrentIndex(WorkMode::CardView);
  ui->table_widget->setRowCount(0);
  ui->table_widget->setColumnCount(1);
  current_account = nullptr;
  ui->table_widget->clearSelection();
  ui->current_label->clear();
  accounts.clear();

  update_grid_test();
  //  if (mode != Company)
  //    update_combobox();

  // style settings
  ui->table_widget->resizeColumnsToContents();
  ui->table_widget->resizeRowsToContents();
  ui->table_widget->setSelectionMode(
      QAbstractItemView::SingleSelection); // only one selection
  ui->table_widget->horizontalHeader()->setStretchLastSection(true);
}

void ClientWindow::update_grid_test() {
  accounts = USER_DB->get_user_accounts(user->get_id());
  banks = USER_DB->get_hash_banks();
  for (size_t i = 0; i < accounts.size(); i++) {
    QString bank_name =
        QString::fromStdString(banks.at(accounts[i]->bank_id)->get_name());
    QTableWidgetItem *item = get_item(accounts[i].get(), bank_name);

    ui->table_widget->insertRow(i);
    ui->table_widget->setItem(i, 0, item);
  }
}

void ClientWindow::update_grid() {
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

    ui->table_widget->insertRow(i);
    ui->table_widget->setItem(i, 0, item);
  }
}

void ClientWindow::init() {
  credit_widget = std::make_unique<CreditWidget>(this);
  transfer_widget = std::make_unique<TransferWidget>(this);
  add_widget = std::make_unique<AddCardWidget>(user.get(), banks, this);
  set_connections();
  ui->stacked_widget->insertWidget(WorkMode::CreditView, credit_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::AddCardView, add_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::TransferView,
                                   transfer_widget.get());
}

void ClientWindow::set_connections() {
  connect(USER_DB, &DataBase::updated, this, &ClientWindow::update);
  connect(credit_widget.get(), &CreditWidget::closed, this,
          &ClientWindow::mode_widget_closed);
  connect(add_widget.get(), &AddCardWidget::closed, this,
          &ClientWindow::mode_widget_closed);
  connect(transfer_widget.get(), &TransferWidget::closed, this,
          &ClientWindow::mode_widget_closed);
}

QTableWidgetItem *ClientWindow::get_item(BankAccount *acc, QString bank) {
  QString item = acc->get_info() + "Bank : " + bank;
  return new QTableWidgetItem(item);
}

void ClientWindow::on_log_out_but_clicked() { this->close(); }

void ClientWindow::on_info_but_clicked() {
  QMessageBox::about(this, "your info", user->get_info());
}

void ClientWindow::on_freeze_but_clicked() {
  if (current_account)
    qDebug() << ui->freeze_but->text()
             << AccountManager::freeze_request(current_account);
}

void ClientWindow::on_withdraw_but_clicked() {
  if (!current_account)
    return;
  transfer_widget->show(TransferWidget::Mode::Withdraw, current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::TransferView);
}

void ClientWindow::on_new_card_but_clicked() {
  add_widget->show();
  ui->stacked_widget->setCurrentIndex(WorkMode::AddCardView);
}

void ClientWindow::on_transfer_but_clicked() {
  if (!current_account)
    return;
  //  switch_widget(false, true, ui->transfer_widget);
  transfer_widget->show(TransferWidget::Mode::Transfer, current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::TransferView);
}

void ClientWindow::on_table_widget_cellClicked(int row, int) {
  current_account = accounts[row].get();
  if (current_account->get_frozen()) {
    ui->freeze_but->setText("Unfreeze");
    ui->transfer_but->setEnabled(false);
    ui->withdraw_but->setEnabled(false);
    ui->credit_but->setEnabled(false);
  } else {
    ui->freeze_but->setText("Freeze");
    ui->transfer_but->setEnabled(true);
    ui->withdraw_but->setEnabled(true);
    ui->credit_but->setEnabled(true);
  }
  ui->current_label->setText("Selected account : " +
                             QString::number(current_account->get_id()));
}

void ClientWindow::mode_widget_closed() {
  ui->stacked_widget->setCurrentIndex(WorkMode::CardView);
}

void ClientWindow::send_add_account(size_t bank_id) {
  std::unique_ptr<BankAccount> to_add(new BankAccount(user->id, bank_id));
  if (AccountManager::add_account_request(to_add.get()))
    QMessageBox::information(this, "add card", "Ur request was send to bank.");
  else
    qDebug() << "Add account : false";
}

void ClientWindow::on_credit_but_clicked() {
  if (current_account)
    ui->stacked_widget->setCurrentIndex(WorkMode::CreditView);
}

// void DepositManager::on_cancel_but_clicked() {
//   switch_widget(true, false, ui->table_widget);
//   ui->id_line->clear();
//   ui->amount_line->clear();
// }

// void DepositManager::on_cance_ac_but_clicked() {
//   switch_widget(true, false, ui->table_widget);
// }

// void DepositManager::on_confirm_ac_but_clicked() {
//   send_add_account(banks[ui->bank_chooser->currentIndex()]);
// }
