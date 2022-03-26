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

void DepositManager::init() { ui->tableWidget->clear(); }
