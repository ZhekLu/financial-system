#include "systemwindow.h"
#include "ui_systemwindow.h"

SystemWindow::SystemWindow(IUser *user, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SystemWindow), user(user) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  // transactions
  transaction_widget = std::make_unique<HistoryWidget>(
      user, Request::Type::TRANSFER, IHistoryManager::ItemsType::SYSTEM, this);
  transaction_requests_widget =
      std::make_unique<HistoryWidget>(user, Request::Type::TRANSFER_REQUEST,
                                      IHistoryManager::ItemsType::SYSTEM, this);
  // loans
  credit_widget = std::make_unique<HistoryWidget>(
      user, Request::Type::CREDIT, IHistoryManager::ItemsType::SYSTEM, this);
  installment_widget =
      std::make_unique<HistoryWidget>(user, Request::Type::INSTALLMENT,
                                      IHistoryManager::ItemsType::SYSTEM, this);
  // states
  accounts_widget =
      std::make_unique<HistoryWidget>(user, Request::Type::LOGIN_ACCOUNT,
                                      IHistoryManager::ItemsType::SYSTEM, this);
  blocks_widget = std::make_unique<HistoryWidget>(
      user, Request::Type::BLOCK, IHistoryManager::ItemsType::SYSTEM, this);
  // adds
  salary_widget = std::make_unique<HistoryWidget>(
      user, Request::Type::SALARY, IHistoryManager::ItemsType::SYSTEM, this);
  deposit_widget = std::make_unique<HistoryWidget>(
      user, Request::Type::DEPOSIT, IHistoryManager::ItemsType::SYSTEM, this);
  // regs
  registration_widget =
      std::make_unique<HistoryWidget>(user, Request::Type::LOGIN_USER,
                                      IHistoryManager::ItemsType::SYSTEM, this);

  ui->tab_widget->insertTab(WidgetType::TRANSACTIONS, transaction_widget.get(),
                            "transactions");
  ui->tab_widget->insertTab(WidgetType::CREDITS, credit_widget.get(),
                            "credits");
  ui->tab_widget->insertTab(WidgetType::INSTALLMENTS, installment_widget.get(),
                            "installment");
  ui->tab_widget->insertTab(WidgetType::ACCOUNTS, accounts_widget.get(),
                            "accounts");
  ui->tab_widget->insertTab(WidgetType::BLOCKS, blocks_widget.get(), "blocks");
  ui->tab_widget->insertTab(WidgetType::SALARIES, salary_widget.get(),
                            "salaries");
  ui->tab_widget->insertTab(WidgetType::DEPOSITS, deposit_widget.get(),
                            "deposits");
  ui->tab_widget->insertTab(WidgetType::TRANSACTIONS_REQUESTS,
                            transaction_requests_widget.get(),
                            "transactions_request");
  ui->tab_widget->insertTab(WidgetType::REGISTRATIONS,
                            registration_widget.get(), "registrations");
  ui->tab_widget->setCurrentIndex(WidgetType::CREDITS);
}

SystemWindow::~SystemWindow() { delete ui; }

void SystemWindow::on_tab_widget_tabBarClicked(int index) {
  bool vis = index == WidgetType::TRANSACTIONS;
  ui->verify_but->setVisible(!vis);
}

void SystemWindow::on_verify_but_clicked() {
  switch (ui->tab_widget->currentIndex()) {
  case WidgetType::CREDITS:
    qDebug() << "Credit verify : " << credit_widget->mark(true);
    break;
  case WidgetType::INSTALLMENTS:
    qDebug() << "Installment verify : " << installment_widget->mark(true);
    break;
  case WidgetType::ACCOUNTS:
    qDebug() << "Account verify : " << accounts_widget->mark(true);
    break;
  case WidgetType::BLOCKS:
    qDebug() << "Block verify : " << blocks_widget->mark(true);
    break;
  case WidgetType::SALARIES:
    qDebug() << "Salary verify : " << salary_widget->mark(true);
    break;
  case WidgetType::DEPOSITS:
    qDebug() << "Deposit verify : " << deposit_widget->mark(true);
    break;
  case WidgetType::TRANSACTIONS_REQUESTS:
    qDebug() << "Transaction request verify : "
             << transaction_requests_widget->mark(true);
    break;
  case WidgetType::REGISTRATIONS:
    qDebug() << "Registration verify : " << registration_widget->mark(true);
    break;
  }
}

void SystemWindow::on_undo_but_clicked() {
  switch (ui->tab_widget->currentIndex()) {
  case WidgetType::TRANSACTIONS:
    qDebug() << "Transaction undo : " << transaction_widget->mark(false);
    break;
  case WidgetType::CREDITS:
    qDebug() << "Credit undo : " << credit_widget->mark(false);
    break;
  case WidgetType::INSTALLMENTS:
    qDebug() << "Installment undo : " << installment_widget->mark(false);
    break;
  case WidgetType::ACCOUNTS:
    qDebug() << "Account undo : " << accounts_widget->mark(false);
    break;
  case WidgetType::BLOCKS:
    qDebug() << "Block undo : " << blocks_widget->mark(false);
    break;
  case WidgetType::SALARIES:
    qDebug() << "Salary undo : " << salary_widget->mark(false);
    break;
  case WidgetType::DEPOSITS:
    qDebug() << "Deposit undo : " << deposit_widget->mark(false);
    break;
  case WidgetType::TRANSACTIONS_REQUESTS:
    qDebug() << "Transaction request undo : "
             << transaction_requests_widget->mark(false);
    break;
  case WidgetType::REGISTRATIONS:
    qDebug() << "Registration undo : " << registration_widget->mark(false);
    break;
  }
}

void SystemWindow::on_info_but_clicked() {}

void SystemWindow::on_log_out_but_clicked() { this->close(); }
