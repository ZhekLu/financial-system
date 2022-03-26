#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "modes.h"
#include "view/authwidget.h"
#include "view/managerfactory.h"

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

  void auth_connection(size_t);

  void on_exit_but_clicked();

  void on_debug_but_clicked();

private:
  // fields
  Ui::MainWindow *ui;
  AuthWidget *aw;

  // methods
  void on_login_button_clicked(LoginMode mode);
};
#endif // MAINWINDOW_H
