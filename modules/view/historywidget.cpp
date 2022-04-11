#include "historywidget.h"
#include "ui_historywidget.h"

HistoryWidget::HistoryWidget(IUser *user, Request::Type request_type,
                             IHistoryManager::ItemsType user_mode,
                             QWidget *parent)
    : QWidget(parent), ui(new Ui::HistoryWidget), type(request_type),
      user_mode(user_mode) {
  ui->setupUi(this);
  manager_factory(user, request_type);

  connect(USER_DB, &DataBase::updated, this, &HistoryWidget::update);
  update();
}

HistoryWidget::~HistoryWidget() { delete ui; }

bool HistoryWidget::mark(bool verified) {
  if (!selection_set())
    return false;
  return manager->mark(current_index, verified);
}

void HistoryWidget::update() {
  // clear
  ui->table_widget->setRowCount(0);
  ui->table_widget->setColumnCount(1);
  ui->table_widget->clearSelection();
  current_index = -1;

  update_grid();

  // style settings
  ui->table_widget->resizeColumnsToContents();
  ui->table_widget->resizeRowsToContents();
  ui->table_widget->setSelectionMode(
      QAbstractItemView::SingleSelection); // only one selection
  ui->table_widget->horizontalHeader()->setStretchLastSection(true);
}

void HistoryWidget::update_grid() {
  ui->table_widget->clear();
  auto items = manager->get_items();
  for (size_t i = 0; i < items.size(); i++) {
    ui->table_widget->insertRow(i);
    ui->table_widget->setItem(i, 0, items[i]);
  }
}

void HistoryWidget::manager_factory(IUser *user, Request::Type request_type) {
  switch (request_type) {
  case Request::TRANSFER:
  case Request::WITHDRAW:
    manager = std::make_unique<TransactionManager>(user);
    break;
  case Request::INSTALLMENT:
    manager = std::make_unique<LoanManager>(LoanManager::LoanType::INSTALLMENT,
                                            user, user_mode);
    break;
  case Request::CREDIT:
    manager = std::make_unique<LoanManager>(LoanManager::LoanType::CREDIT, user,
                                            user_mode);
    break;
  case Request::LOGIN_ACCOUNT:
    manager = std::make_unique<AccountManager>(
        user, AccountManager::Mode::LOGIN, user_mode);
    break;
  case Request::LOGIN_USER:
    break;
  case Request::BLOCK:
  case Request::UNBLOCK:
    manager = std::make_unique<AccountManager>(
        user, AccountManager::Mode::BLOCK, user_mode);
    break;
  default:
    break;
  }
}

void HistoryWidget::on_table_widget_cellClicked(int row, int) {
  current_index = row;
}
