#ifndef SYSTEMWINDOW_H
#define SYSTEMWINDOW_H

#include "historywidget.h"
#include <QMainWindow>

namespace Ui {
class SystemWindow;
}

class SystemWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit SystemWindow(IUser *user, QWidget *parent = nullptr);
  ~SystemWindow();

private slots:
  void on_tab_widget_tabBarClicked(int index);
  void on_verify_but_clicked();
  void on_undo_but_clicked();
  void on_info_but_clicked();
  void on_log_out_but_clicked();

private:
  enum WidgetType { TRANSACTIONS, CREDITS, INSTALLMENTS, ACCOUNTS, BLOCKS };

  Ui::SystemWindow *ui;
  std::unique_ptr<IUser> user;

  std::unique_ptr<HistoryWidget> transaction_widget;
  std::unique_ptr<HistoryWidget> credit_widget;
  std::unique_ptr<HistoryWidget> installment_widget;
  std::unique_ptr<HistoryWidget> accounts_widget;
  std::unique_ptr<HistoryWidget> blocks_widget;
};

#endif // SYSTEMWINDOW_H
