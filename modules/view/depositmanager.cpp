#include "depositmanager.h"
#include "ui_depositmanager.h"

DepositManager::DepositManager(Individual *owner, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DepositManager), user(owner) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  init();
}

DepositManager::~DepositManager() {
  delete ui;
  delete user;
}

void DepositManager::init() {
  ui->tableWidget->clear();
  ui->tableWidget->setColumnCount(1);

  QSqlQuery query;

  query.exec("select id, bank_id, balance from accounts;");

  for (int i = 0; query.next(); i++) {
    QString id = query.value(0).toString();
    QString bank_id = query.value(1).toString();
    QString balance = query.value(2).toString();

    QString res = id + " " + bank_id + "\n" + balance;
    qDebug() << i << res;

    ui->tableWidget->insertRow(i);
    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(res));
  }
  ui->tableWidget->resizeColumnsToContents();
  ui->tableWidget->resizeRowsToContents();
  ui->tableWidget->setSelectionMode(
      QAbstractItemView::SingleSelection); // only one selection
  //  ui->tableWidget->hideColumn(0);          // hide index
  ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}
