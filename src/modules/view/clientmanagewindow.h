#ifndef CLIENTMANAGEWINDOW_H
#define CLIENTMANAGEWINDOW_H

#include "historywidget.h"
#include <QMainWindow>

namespace Ui {
class UserManageWindow;
}

class ClientManageWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit ClientManageWindow(IUser *user, QWidget *parent = nullptr);
  ~ClientManageWindow();

signals:
  void closed();

private slots:
  void on_back_but_clicked();
  void on_pay_buy_clicked();

private:
  enum WidgetType { INSTALLMENT, CREDITS, ACCOUNTS };

  Ui::UserManageWindow *ui;
  IUser *user;

  std::unique_ptr<HistoryWidget> credit_widget;
  std::unique_ptr<HistoryWidget> installment_widget;
};

#endif // CLIENTMANAGEWINDOW_H
