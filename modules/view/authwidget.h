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
  AuthWidget(QWidget *parent = nullptr);
  ~AuthWidget();

  // methods
  virtual void show(LoginMode login_mode);
  virtual void hide();

signals:
  void auth_ok(size_t);
private slots:

  void on_enter_btn_clicked();

  void on_back_btn_clicked();

private:
  Ui::AuthWidget *ui;
  LoginMode mode;
};

#endif // AUTHWIDGET_H
