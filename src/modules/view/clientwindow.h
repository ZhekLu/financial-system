#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "accountswidget.h"
#include "addcardwidget.h"
#include "additionwidget.h"
#include "clientmanagewindow.h"
#include "creditwidget.h"
#include "historywidget.h"
#include "modules/database/userdb.h"
#include "modules/entities/bank/accountstatemanager.h"
#include "modules/entities/bank/bankaccount.h"
#include "modules/entities/entity.h"
#include "modules/entities/individual.h"
#include "periodwidget.h"
#include "transferwidget.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidget>
#include <memory>
#include <unordered_map>

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
  void on_block_but_clicked();
  void on_credit_but_clicked();
  void on_installment_but_clicked();
  void on_deposit_but_clicked();
  void on_info_but_clicked();
  void on_log_out_but_clicked();
  void on_manage_but_clicked();

  // Widgets
  void account_selected(BankAccount *);
  void mode_widget_closed();
  void manage_window_closed();

private:
  // View

  enum WorkMode {
    CardView,
    AdditionView,
    TransferView,
    CreditView,
    AddCardView,
    CreditPayView
  };

  Ui::ClientWindow *ui;

  // Mode widgets

  std::unique_ptr<AccountsWidget> card_widget;
  std::unique_ptr<TransferWidget> transfer_widget;
  std::unique_ptr<CreditWidget> credit_widget;
  std::unique_ptr<AddCardWidget> add_card_widget;
  std::unique_ptr<AdditionWidget> additions_widget;
  std::unique_ptr<ClientManageWindow> manage_window;

  // fields

  std::unique_ptr<IUser> user;
  AccessMode mode;
  BankAccount *current_account;

  // methods

  void update();
  void update_buttons();
  void clear_selected();

  void init();
  void init_widgets();
  void init_person_widgets();
  void init_company_widgets();
  void init_stack();
  void set_connections();
  void init_buttons();

  void person_card_request();
  void company_card_request();
};

#endif // CLIENTWINDOW_H
