#include "authwidget.h"
#include "ui_authwidget.h"

using namespace std;

AuthWidget::AuthWidget(LoginMode mode, QWidget *parent)
    : QWidget(parent), ui(new Ui::AuthWidget), mode(mode) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
}

AuthWidget::~AuthWidget() { delete ui; }

void AuthWidget::on_enter_btn_clicked() {
  string login = ui->login->text().toStdString();
  string password = ui->password->text().toStdString();

  //  if (USER_DB->contains(SystemUser(login, password, lm)))
  //          QMessageBox::critical(this, "Authorization", "Wrong login or
  //          password");
  //      else
  //          this->close();

  //      ui->login->clear();
  //      ui->password->clear();
}

void AuthWidget::on_back_btn_clicked() { this->close(); }
