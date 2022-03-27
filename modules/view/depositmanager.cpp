#include "depositmanager.h"
#include "ui_depositmanager.h"

DepositManager::DepositManager(IUser *owner, Mode mode, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DepositManager), user(owner),
      access_rights(mode) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  init_grid();
  connect(USER_DB, &DataBase::updated, this, &DepositManager::init_grid);
}

DepositManager::~DepositManager() {
  delete ui;
  delete user;
}

void DepositManager::init_grid() {
  ui->tableWidget->setRowCount(0);
  ui->tableWidget->setColumnCount(1);

  // fill with data
  QSqlQuery query;
  query.exec(QString("select id, name, balance "
                     "from accounts inner join companies "
                     "on accounts.bank_id=companies.BIC where user_id=%1 ;")
                 .arg(QString::number(user->id)));
  for (int i = 0; query.next(); i++) {
    QTableWidgetItem *item =
        get_item(query.value(0).toString(), query.value(1).toString(),
                 query.value(2).toString());

    ui->tableWidget->insertRow(i);
    ui->tableWidget->setItem(i, 0, item);
  }

  // style settings
  ui->tableWidget->resizeColumnsToContents();
  ui->tableWidget->resizeRowsToContents();
  ui->tableWidget->setSelectionMode(
      QAbstractItemView::SingleSelection); // only one selection
  ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

QTableWidgetItem *DepositManager::get_item(QString account_id, QString bank,
                                           QString balance) {
  QString item = QString("Card : %1\nBalance : %2\nBank : %3")
                     .arg(account_id, balance, bank);
  return new QTableWidgetItem(item);
}
