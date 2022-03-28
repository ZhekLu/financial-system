#ifndef DEPOSITMANAGER_H
#define DEPOSITMANAGER_H

#include "creditmanager.h"
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

  // main windows
  void on_log_out_but_clicked();
  void on_info_but_clicked();
  void on_freeze_but_clicked();
  void on_withdraw_but_clicked();
  void on_new_card_but_clicked();
  void on_transfer_but_clicked();

  // table
  void on_table_widget_cellClicked(int row, int column);

  // transfer & withdraw
  void on_confirm_but_clicked();
  void on_cancel_but_clicked();

  // add
  void on_cance_ac_but_clicked();
  void on_confirm_ac_but_clicked();

  void on_credit_but_clicked();

private:
  Ui::DepositManager *ui;
  std::unique_ptr<CreditManager> credit_widget;

  std::unique_ptr<IUser> user;
  Mode mode;

  std::vector<std::unique_ptr<BankAccount>> accounts;
  std::vector<size_t> banks;
  BankAccount *current_account;

  std::unique_ptr<QRegularExpressionValidator> card_validator;
  std::unique_ptr<QIntValidator> amount_validator;

  // methods
  void update();
  void update_grid();
  void update_combobox();
  void init();
  QTableWidgetItem *get_item(BankAccount *, QString);

  void switch_widget(bool, bool, QWidget *);
  void send_add_account(size_t);
};

#endif // DEPOSITMANAGER_H
