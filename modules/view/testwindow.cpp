#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::TestWindow) {
  ui->setupUi(this);
  aw = new AddCardWidget();
  tf = new TransferWidget();
  ui->stackedWidget->insertWidget(1, aw);
  ui->stackedWidget->insertWidget(2, tf);
}

TestWindow::~TestWindow() { delete ui; }

void TestWindow::on_pushButton_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
}

void TestWindow::on_pushButton_2_clicked() {
  ui->stackedWidget->setCurrentIndex(2);
}
