#include "systemwindow.h"
#include "ui_systemwindow.h"

SystemWindow::SystemWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SystemWindow) {
  ui->setupUi(this);

  history_widget = std::make_unique<HistoryWidget>(Request::Type::TRANSFER);
  credit_widget = std::make_unique<HistoryWidget>(Request::Type::CREDIT);

  ui->tab_widget->addTab(history_widget.get(), "transactions");
  ui->tab_widget->addTab(credit_widget.get(), "credits");
}

SystemWindow::~SystemWindow() { delete ui; }
