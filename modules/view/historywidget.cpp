#include "historywidget.h"
#include "ui_historywidget.h"

HistoryWidget::HistoryWidget(IUser *user, Request::Type request_type,
                             QWidget *parent)
    : QWidget(parent), ui(new Ui::HistoryWidget), type(request_type) {
  ui->setupUi(this);
  connect(USER_DB, &DataBase::updated, this, &HistoryWidget::update);

  switch (request_type) {
  case Request::TRANSFER:
  case Request::WITHDRAW:
  case Request::TOPUP:
    manager = std::make_unique<AccountManager>(user);
    break;
  case Request::INSTALLMENT:
  case Request::CREDIT:
    manager = std::make_unique<CreditManager>(user);
    break;
  case Request::LOGIN_ACCOUNT:
    break;
  case Request::LOGIN_USER:
    break;
  default:
    break;
  }
  update();
}

HistoryWidget::~HistoryWidget() { delete ui; }

bool HistoryWidget::mark(bool verified) {
  if (!selection_set())
    return false;
  return verified ? manager->verify(current_index)
                  : manager->undo(current_index);
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

void HistoryWidget::on_table_widget_cellClicked(int row, int) {
  current_index = row;
}
