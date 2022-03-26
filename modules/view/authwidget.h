#ifndef AUTHWIDGET_H
#define AUTHWIDGET_H

#include <QMessageBox>
#include <QWidget>

//#include "financialsystem.h"
#include "modules/database/userdb.h"
#include "modules/modes.h"

namespace Ui {
class AuthWidget;
}

class AuthWidget : public QWidget {
  Q_OBJECT

public:
  AuthWidget(LoginMode mode, QWidget *parent = nullptr);
  ~AuthWidget();
private slots:
  void on_enter_btn_clicked();

  void on_back_btn_clicked();

private:
  Ui::AuthWidget *ui;
  LoginMode mode;
};

#endif // AUTHWIDGET_H
