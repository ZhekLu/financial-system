#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_read_bt_clicked() { USER_DB->print_all_users(); }

void MainWindow::on_write_bt_clicked() {
  std::vector<SystemUser> su{
      SystemUser("log1", "pas1", LoginMode::INDIVIDUAL, 1),
      SystemUser("log2", "pas12", LoginMode::INDIVIDUAL, 2),
      SystemUser("log3", "pas13", LoginMode::INDIVIDUAL, 13),
      SystemUser("log4", "pas14", LoginMode::INDIVIDUAL, 14),
      SystemUser("log5", "pas15", LoginMode::INDIVIDUAL, 5),
      SystemUser("log6", "pas16", LoginMode::INDIVIDUAL, 16),
      SystemUser("log7", "pas17", LoginMode::INDIVIDUAL, 17)};
  for (auto &s : su) {
    USER_DB->login_user(s);
  }
  //  USER_DB->test_login();
}
