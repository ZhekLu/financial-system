#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  aw = new AuthWidget();
  rw = std::make_unique<RegistrationWidget>();
  connect(aw, &AuthWidget::auth_ok, this, &MainWindow::auth_connection);
  ui->debug_but->setVisible(false);
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
  USER_DB->print_mode_system_users(mode);
}

void MainWindow::on_exit_but_clicked() { this->close(); }

void MainWindow::on_reg_but_clicked() { rw->show(); }

void MainWindow::on_debug_but_clicked() {
  //    IdGenerator::GenerateId();
  //    ClientWindow* cl = new ClientWindow()
}

void MainWindow::auth_connection(size_t id, LoginMode mode) {
  QMainWindow *manager = ManagerFactory::get_manager_widget(id, mode, this);
  if (manager)
    manager->show();
  else
    qDebug() << "manager factory fail.";
}
