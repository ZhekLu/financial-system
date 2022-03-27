#ifndef DEPOSITMANAGER_H
#define DEPOSITMANAGER_H

#include "modules/database/userdb.h"
#include "modules/entities/bank/accountmanager.h"
#include "modules/entities/bank/bankaccount.h"
#include "modules/entities/entity.h"
#include "modules/entities/individual.h"

#include <QDebug>
#include <QIntValidator>
#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <memory>

namespace Ui {
class DepositManager;
} // namespace Ui

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

  void on_confirm_but_clicked();

  void on_cancel_but_clicked();

private:
  Ui::DepositManager *ui;

  std::unique_ptr<IUser> user;
  Mode access_rights;

  std::vector<std::unique_ptr<BankAccount>> accounts;
  BankAccount *current_account;

  std::unique_ptr<QRegularExpressionValidator> card_validator;
  std::unique_ptr<QIntValidator> amount_validator;

  // methods
  void update();
  void update_grid();
  void init();
  QTableWidgetItem *get_item(BankAccount *, QString);

  void switch_widget(bool table, bool transfer = true);
  void clean_transfer_widget();
};

#endif // DEPOSITMANAGER_H
