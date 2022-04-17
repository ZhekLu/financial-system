#ifndef ACCOUNTSWIDGET_H
#define ACCOUNTSWIDGET_H

#include "modules/entities/bank/accountmanager.h"
#include <QWidget>

namespace Ui {
class AccountsWidget;
}

class AccountsWidget : public QWidget {
  Q_OBJECT

public:
  explicit AccountsWidget(IUser *user, QWidget *parent = nullptr);
  ~AccountsWidget();

signals:
  void selected(BankAccount *);

private slots:
  void on_table_widget_cellClicked(int row, int column);

private:
  Ui::AccountsWidget *ui;
  std::unique_ptr<AccountManager> manager;

  void init();
  void update();
  void update_grid();
  void clear_selected();
};

#endif // ACCOUNTSWIDGET_H
