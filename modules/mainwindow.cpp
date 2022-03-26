#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ind_log_but_clicked()
{
    on_login_button_clicked(LoginMode::INDIVIDUAL);
}

void MainWindow::on_ent_log_but_clicked()
{
    on_login_button_clicked(LoginMode::ENTITY);
}

void MainWindow::on_man_log_but_clicked()
{
    on_login_button_clicked(LoginMode::MANAGER);
}

void MainWindow::on_op_log_but_clicked()
{
    on_login_button_clicked(LoginMode::OPERATOR);
}

void MainWindow::on_adm_log_but_clicked()
{
    on_login_button_clicked(LoginMode::ADMIN);
}

void MainWindow::on_login_button_clicked(LoginMode mode)
{
    AuthWidget* aw = new AuthWidget(mode);
    aw->show();
    QString title;
    switch (mode) {
    case LoginMode::ADMIN:
        title = "System::Admin";
        break;
    case LoginMode::ENTITY:
        title = "Client::Entity";
        break;
    case LoginMode::MANAGER:
        title = "System::Manager";
        break;
    case LoginMode::OPERATOR:
        title = "System::Operator";
        break;
    case LoginMode::INDIVIDUAL:
        title = "Client::Individual";
        break;
    }
    aw->setWindowTitle(title);
}
