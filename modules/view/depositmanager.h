#ifndef DEPOSITMANAGER_H
#define DEPOSITMANAGER_H

#include "modules/database/userdb.h"
#include "modules/entities/bank/accountmanager.h"
#include "modules/entities/bank/bankaccount.h"
#include "modules/entities/entity.h"
#include "modules/entities/individual.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <memory>

namespace Ui {
class DepositManager;
}

class DepositManager : public QMainWindow {
  Q_OBJECT

public:
  enum Mode { Company, Person };
  DepositManager(IUser *owner, Mode mode, QWidget *parent = nullptr);
  ~DepositManager();

private slots:

  void on_log_out_but_clicked();

  void on_info_but_clicked();

  void on_freeze_but_clicked();

  void on_withdraw_but_clicked();

  void on_new_card_but_clicked();

  void on_transfer_but_clicked();

  void on_tableWidget_cellClicked(int row, int column);

private:
  Ui::DepositManager *ui;

  IUser *user;
  Mode access_rights;
  std::vector<std::unique_ptr<BankAccount>> accounts;
  BankAccount *current_account;

  void update();
  void update_grid();
  QTableWidgetItem *get_item(BankAccount *, QString);
};

#endif // DEPOSITMANAGER_H
