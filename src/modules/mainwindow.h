#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "modules/modes.h"
#include "view/authwidget.h"
#include "view/clientwindow.h"
#include "view/registrationwidget.h"
#include "view/systemwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_ind_log_but_clicked();
  void on_ent_log_but_clicked();
  void on_man_log_but_clicked();
  void on_op_log_but_clicked();
  void on_adm_log_but_clicked();
  void on_exit_but_clicked();
  void on_debug_but_clicked();
  void on_reg_but_clicked();

  void auth_connection(size_t, LoginMode);
  void on_login_button_clicked(LoginMode mode);

private:
  // fields
  Ui::MainWindow *ui;
  AuthWidget *aw;
  std::unique_ptr<RegistrationWidget> rw;

  // methods
  QMainWindow *manager_factory(size_t id, LoginMode mode);
  IUser *get_user(size_t id, LoginMode mode);
};
#endif // MAINWINDOW_H
