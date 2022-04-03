#include "historywidget.h"
#include "ui_historywidget.h"

HistoryWidget::HistoryWidget(Request::Type request_type, QWidget *parent)
    : QWidget(parent), ui(new Ui::HistoryWidget), type(request_type) {
  ui->setupUi(this);
  connect(USER_DB, &DataBase::updated, this, &HistoryWidget::update);

  switch (request_type) {
  case Request::TRANSFER:
  case Request::WITHDRAW:
  case Request::TOPUP:
    manager = std::make_unique<TransferManager>();
    break;
  case Request::FREEZE:
    break;
  case Request::CREDIT:
    manager = std::make_unique<CreditManager>();
    break;
  case Request::INSTALLMENT:
    break;
  case Request::LOGIN_ACCOUNT:
    break;
  case Request::LOGIN_USER:
    break;
  }
  update();
}

HistoryWidget::~HistoryWidget() { delete ui; }

void HistoryWidget::update() {
  // clear
  ui->table_widget->setRowCount(0);
  ui->table_widget->setColumnCount(1);

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
  int index = 0;
  for (auto *item : manager->get_items()) {
    ui->table_widget->insertRow(index);
    ui->table_widget->setItem(index, 0, item);
  }
}

void HistoryWidget::on_table_widget_cellClicked(int row, int) {
  //    ui->table_widget->currentRow()
}
