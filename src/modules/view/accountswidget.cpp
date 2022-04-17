#include "accountswidget.h"
#include "ui_accountswidget.h"

AccountsWidget::AccountsWidget(IUser *user, QWidget *parent)
    : QWidget(parent), ui(new Ui::AccountsWidget) {
  ui->setupUi(this);
  manager = std::make_unique<AccountManager>(user);
  init();
}

AccountsWidget::~AccountsWidget() { delete ui; }

void AccountsWidget::clear_selection() { ui->table_widget->clearSelection(); }

void AccountsWidget::on_table_widget_cellClicked(int row, int) {
  BankAccount *curr = manager->get_selected_account(row);
  emit selected(curr);
}

void AccountsWidget::init() {
  connect(USER_DB, &DataBase::updated, this, &AccountsWidget::update);
  update();
}

void AccountsWidget::update() {
  // clear
  ui->table_widget->setRowCount(0);
  ui->table_widget->setColumnCount(1);
  clear_selected();

  update_grid();

  // style settings
  ui->table_widget->resizeColumnsToContents();
  ui->table_widget->resizeRowsToContents();
  ui->table_widget->setSelectionMode(
      QAbstractItemView::SingleSelection); // only one selection
  ui->table_widget->horizontalHeader()->setStretchLastSection(true);
}

void AccountsWidget::update_grid() {
  ui->table_widget->clear();
  auto items = manager->get_items();
  for (size_t i = 0; i < items.size(); i++) {
    ui->table_widget->insertRow(i);
    ui->table_widget->setItem(i, 0, items[i]);
  }
}

void AccountsWidget::clear_selected() {
  ui->table_widget->clearSelection();
  emit selected(nullptr);
}
