#include "authwidget.h"
#include "ui_authwidget.h"

using namespace std;

AuthWidget::AuthWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AuthWidget) {
  ui->setupUi(this);
  //  setAttribute(Qt::WA_DeleteOnClose);
}

AuthWidget::~AuthWidget() { delete ui; }

void AuthWidget::show(LoginMode login_mode) {
  QWidget::show();
  mode = login_mode;

  switch (mode) {
  case LoginMode::ADMIN:
    setWindowTitle("System::Admin");
    break;
  case LoginMode::ENTITY:
    setWindowTitle("Client::Entity");
    break;
  case LoginMode::MANAGER:
    setWindowTitle("System::Manager");
    break;
  case LoginMode::OPERATOR:
    setWindowTitle("System::Operator");
    break;
  case LoginMode::INDIVIDUAL:
    setWindowTitle("Client::Individual");
    break;
  }
}

void AuthWidget::hide() {
  QWidget::hide();
  ui->login->clear();
  ui->password->clear();
}

void AuthWidget::on_enter_btn_clicked() {
  QString login = ui->login->text();
  QString password = ui->password->text();

  size_t user = LoginManager::get_user(login, password, mode);
  if (!user) {
    QMessageBox::critical(this, "Authorization", "Wrong login or password");
  } else {
    qDebug() << "auth ok";
    emit auth_ok(user, mode);
    hide();
  }
}

void AuthWidget::on_back_btn_clicked() { this->hide(); }
