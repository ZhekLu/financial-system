#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  aw = new AuthWidget();
  connect(aw, &AuthWidget::auth_ok, this, &MainWindow::connection_test);
}

MainWindow::~MainWindow() {
  delete ui;
  delete aw;
}

void MainWindow::on_ind_log_but_clicked() {
  on_login_button_clicked(LoginMode::INDIVIDUAL);
}

void MainWindow::on_ent_log_but_clicked() {
  on_login_button_clicked(LoginMode::ENTITY);
}

void MainWindow::on_man_log_but_clicked() {
  on_login_button_clicked(LoginMode::MANAGER);
}

void MainWindow::on_op_log_but_clicked() {
  on_login_button_clicked(LoginMode::OPERATOR);
}

void MainWindow::on_adm_log_but_clicked() {
  on_login_button_clicked(LoginMode::ADMIN);
}

void MainWindow::on_login_button_clicked(LoginMode mode) {
  aw->show(mode);
  USER_DB->print_all_users();
}

void MainWindow::connection_test(LoginMode mode) {
  this->setWindowTitle("GOOOG" + QString::number(mode));
  QMessageBox::information(this, "lf", "gg");
}
