#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "addcardwidget.h"
#include "creditwidget.h"
#include "modules/database/userdb.h"
#include "modules/entities/bank/accountmanager.h"
#include "modules/entities/bank/bankaccount.h"
#include "modules/entities/entity.h"
#include "modules/entities/individual.h"
#include "transferwidget.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <memory>

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow {
  Q_OBJECT

public:
  enum AccessMode { Company, Person };
  ClientWindow(IUser *owner, AccessMode mode, QWidget *parent = nullptr);
  ~ClientWindow();

private slots:
  // Buttons
  void on_new_card_but_clicked();
  void on_transfer_but_clicked();
  void on_withdraw_but_clicked();
  void on_freeze_but_clicked();
  void on_credit_but_clicked();
  void on_info_but_clicked();
  void on_log_out_but_clicked();
  // Widgets
  void on_table_widget_cellClicked(int, int);
  void mode_widget_closed();

private:
  // View
  enum WorkMode { CardView, CreditView, AddCardView, TransferView };
  Ui::ClientWindow *ui;

  // Mode widgets
  std::unique_ptr<TransferWidget> transfer_widget;
  std::unique_ptr<CreditWidget> credit_widget;
  std::unique_ptr<AddCardWidget> add_widget;

  // methods
  std::unique_ptr<IUser> user;
  AccessMode mode;

  std::vector<std::unique_ptr<BankAccount>> accounts;
  std::vector<size_t> banks;
  BankAccount *current_account;

  std::unique_ptr<QRegularExpressionValidator> card_validator;
  std::unique_ptr<QIntValidator> amount_validator;

  // methods
  void update();
  void update_grid();
  //  void update_combobox();
  void init();
  void set_connections();
  QTableWidgetItem *get_item(BankAccount *, QString);

  //  void switch_widget(bool, bool, QWidget *);
  void send_add_account(size_t);
};

#endif // CLIENTWINDOW_H
