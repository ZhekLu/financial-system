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
  clear_selected();
  accounts.clear();

  update_grid_test();

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
    QString bank_name = banks.at(accounts[i]->get_bank())->get_name();
    QTableWidgetItem *item = get_item(accounts[i].get(), bank_name);

    ui->table_widget->insertRow(i);
    ui->table_widget->setItem(i, 0, item);
  }
}

void ClientWindow::clear_selected() {
  current_account = nullptr;
  ui->table_widget->clearSelection();
  ui->current_label->clear();
}

void ClientWindow::update_grid() {
  // fill with data
  QSqlQuery query;
  query.exec(QString("select id, bank_id, balance, frozen, name "
                     "from accounts inner join companies "
                     "on accounts.bank_id=companies.BIC where user_id=%1 ;")
                 .arg(QString::number(user->get_id())));
  for (int i = 0; query.next(); i++) {

    size_t id = query.value(0).toULongLong();
    size_t bank_id = query.value(1).toULongLong();
    size_t balance = query.value(2).toULongLong();
    bool frozen = query.value(3).toBool();
    QString bank_name = query.value(4).toString();
    accounts.push_back(std::make_unique<BankAccount>(id, user->get_id(),
                                                     bank_id, balance, frozen));

    QTableWidgetItem *item = get_item(accounts.back().get(), bank_name);

    ui->table_widget->insertRow(i);
    ui->table_widget->setItem(i, 0, item);
  }
}

void ClientWindow::init() {
  add_widget = std::make_unique<AddCardWidget>(user.get(), banks, this);
  credit_widget = std::make_unique<CreditWidget>(user.get(), banks, this);
  transfer_widget = std::make_unique<TransferWidget>(user.get(), this);
  credit_pay_widget =
      std::make_unique<HistoryWidget>(user.get(), Request::CREDIT, true, this);
  set_connections();
  ui->stacked_widget->insertWidget(WorkMode::CreditView, credit_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::AddCardView, add_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::TransferView,
                                   transfer_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::CreditPayView,
                                   credit_pay_widget.get());
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
             << AccountManager::freeze_request(user->get_id(),
                                               current_account->get_id(),
                                               !current_account->is_frozen());
}

void ClientWindow::on_withdraw_but_clicked() {
  if (!current_account)
    return;
  transfer_widget->show(TransferWidget::Mode::Withdraw, current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::TransferView);
}

void ClientWindow::on_new_card_but_clicked() {
  clear_selected();
  add_widget->show();
  ui->stacked_widget->setCurrentIndex(WorkMode::AddCardView);
}

void ClientWindow::on_transfer_but_clicked() {
  if (!current_account)
    return;
  transfer_widget->show(TransferWidget::Mode::Transfer, current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::TransferView);
}

void ClientWindow::on_table_widget_cellClicked(int row, int) {
  current_account = accounts[row].get();
  if (current_account->is_frozen()) {
    ui->freeze_but->setText("Unfreeze");
    ui->transfer_but->setEnabled(false);
    ui->withdraw_but->setEnabled(false);
  } else {
    ui->freeze_but->setText("Freeze");
    ui->transfer_but->setEnabled(true);
    ui->withdraw_but->setEnabled(true);
  }
  ui->current_label->setText("Selected account : " +
                             QString::number(current_account->get_id()));
}

void ClientWindow::mode_widget_closed() {
  ui->stacked_widget->setCurrentIndex(WorkMode::CardView);
}

void ClientWindow::on_credit_but_clicked() {
  clear_selected();
  credit_widget->show(LoanManager::CREDIT);
  ui->stacked_widget->setCurrentIndex(WorkMode::CreditView);
}

void ClientWindow::on_installment_but_clicked() {
  clear_selected();
  credit_widget->show(LoanManager::INSTALLMENT);
  ui->stacked_widget->setCurrentIndex(WorkMode::CreditView);
}
