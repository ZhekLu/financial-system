#include "entityclientwindow.h"
#include "ui_entityclientwindow.h"

EntityClientWindow::EntityClientWindow(Entity *owner, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EntityClientWindow), user(owner) {
  ui->setupUi(this);
}

EntityClientWindow::~EntityClientWindow() { delete ui; }

void EntityClientWindow::on_new_card_but_clicked() {
  qDebug() << "Add account : "
           << AccountManager::add_account_request(user->get_id(),
                                                  user->get_bank());
}

void EntityClientWindow::on_transfer_but_clicked() {}

void EntityClientWindow::on_deposit_but_clicked() {}

void EntityClientWindow::on_info_but_clicked() {
  QMessageBox::about(this, "your info", user->get_info());
}

void EntityClientWindow::on_log_out_but_clicked() {}
