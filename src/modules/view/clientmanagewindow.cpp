#include "clientmanagewindow.h"
#include "ui_clientmanagewindow.h"

ClientManageWindow::ClientManageWindow(IUser *user, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::UserManageWindow), user(user) {
  ui->setupUi(this);

  installment_widget =
      std::make_unique<HistoryWidget>(user, Request::Type::INSTALLMENT,
                                      IHistoryManager::ItemsType::CLIENT, this);
  credit_widget = std::make_unique<HistoryWidget>(
      user, Request::Type::CREDIT, IHistoryManager::ItemsType::CLIENT, this);

  ui->tab_widget->insertTab(WidgetType::INSTALLMENT, installment_widget.get(),
                            "installment");
  ui->tab_widget->insertTab(WidgetType::CREDITS, credit_widget.get(),
                            "credits");
  ui->tab_widget->setCurrentIndex(WidgetType::CREDITS);
}

ClientManageWindow::~ClientManageWindow() { delete ui; }

void ClientManageWindow::on_back_but_clicked() { emit closed(); }

void ClientManageWindow::on_pay_buy_clicked() {}
