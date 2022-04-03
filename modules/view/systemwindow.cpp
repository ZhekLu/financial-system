#include "systemwindow.h"
#include "ui_systemwindow.h"

SystemWindow::SystemWindow(IUser *user, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SystemWindow), user(user) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  transaction_widget =
      std::make_unique<HistoryWidget>(user, Request::Type::TRANSFER);
  credit_widget = std::make_unique<HistoryWidget>(user, Request::Type::CREDIT);

  ui->tab_widget->insertTab(WidgetType::TRANSACTIONS, transaction_widget.get(),
                            "transactions");
  ui->tab_widget->insertTab(WidgetType::CREDITS, credit_widget.get(),
                            "credits");
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
  case WidgetType::ACCOUNTS:
    break;
  }
}

void SystemWindow::on_undo_but_clicked() {
  switch (ui->tab_widget->currentIndex()) {
  case WidgetType::TRANSACTIONS:
    qDebug() << "Transaction undo : " << transaction_widget->mark(false);
  case WidgetType::CREDITS:
    qDebug() << "Credit undo : " << credit_widget->mark(false);
  case WidgetType::ACCOUNTS:
    break;
  }
}

void SystemWindow::on_info_but_clicked() {}

void SystemWindow::on_log_out_but_clicked() { this->close(); }
