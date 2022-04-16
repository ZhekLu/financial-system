#include "entityclientwindow.h"
#include "ui_entityclientwindow.h"

EntityClientWindow::EntityClientWindow(Entity *owner, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EntityClientWindow), user(owner) {
  ui->setupUi(this);
  init();
  update();
}

EntityClientWindow::~EntityClientWindow() { delete ui; }

// Init

void EntityClientWindow::init() {
  salary_widget = std::make_unique<AdditionWidget>(user.get(), this);
  transfer_widget = std::make_unique<TransferWidget>(
      user.get(), TransferWidget::Type::Company, this);

  set_connections();

  ui->stacked_widget->insertWidget(WorkMode::SalaryView, salary_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::TransferView,
                                   transfer_widget.get());
}

void EntityClientWindow::set_connections() {
  connect(USER_DB, &DataBase::updated, this, &EntityClientWindow::update);
  // close events
  connect(salary_widget.get(), &AdditionWidget::closed, this,
          &EntityClientWindow::mode_widget_closed);
  connect(transfer_widget.get(), &TransferWidget::closed, this,
          &EntityClientWindow::mode_widget_closed);
}

// Buttons

void EntityClientWindow::on_new_card_but_clicked() {
  qDebug() << "Add account : "
           << AccountStateManager::add_account_request(user->get_id(),
                                                  user->get_bank());
}

void EntityClientWindow::on_transfer_but_clicked() {
  if (!current_account)
    return;

  transfer_widget->show(TransferWidget::Mode::Transfer, current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::TransferView);
}

void EntityClientWindow::on_salary_but_clicked() {
  salary_widget->show();
  ui->stacked_widget->setCurrentIndex(WorkMode::SalaryView);
}

void EntityClientWindow::on_info_but_clicked() {
  QMessageBox::about(this, "your info", user->get_info());
}

void EntityClientWindow::on_log_out_but_clicked() { this->close(); }

// Other

void EntityClientWindow::mode_widget_closed() {
  ui->stacked_widget->setCurrentIndex(WorkMode::CardView);
}

void EntityClientWindow::on_table_widget_cellClicked(int row, int) {
  current_account = accounts[row].get();
  bool vis = current_account->is_available();

  ui->transfer_but->setEnabled(vis);
  ui->withdraw_but->setEnabled(vis);

  ui->current_label->setText("Selected account : " +
                             QString::number(current_account->get_id()));
}

// Update View

void EntityClientWindow::update() {
  // clear
  ui->stacked_widget->setCurrentIndex(WorkMode::CardView);
  ui->table_widget->setRowCount(0);
  ui->table_widget->setColumnCount(1);
  clear_selected();
  accounts.clear();

  update_grid();

  // style settings
  ui->table_widget->resizeColumnsToContents();
  ui->table_widget->resizeRowsToContents();
  ui->table_widget->setSelectionMode(
      QAbstractItemView::SingleSelection); // only one selection
  ui->table_widget->horizontalHeader()->setStretchLastSection(true);
}

void EntityClientWindow::update_variables() {
  accounts = USER_DB->get_user_accounts(user->get_id());
  user_bank = USER_DB->get_bank(user->get_bank());
}

void EntityClientWindow::update_grid() {
  update_variables();
  for (size_t i = 0; i < accounts.size(); i++) {
    QTableWidgetItem *item = get_item(accounts[i].get(), user_bank->get_name());

    ui->table_widget->insertRow(i);
    ui->table_widget->setItem(i, 0, item);
  }
}

void EntityClientWindow::clear_selected() {
  current_account = nullptr;
  ui->table_widget->clearSelection();
  ui->current_label->clear();
}

QTableWidgetItem *EntityClientWindow::get_item(BankAccount *acc, QString bank) {
  QString item = acc->get_info() + "Bank : " + bank;
  return new QTableWidgetItem(item);
}
