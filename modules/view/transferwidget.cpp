#include "transferwidget.h"
#include "ui_transferwidget.h"

TransferWidget::TransferWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::TransferWidget) {
  ui->setupUi(this);
}

TransferWidget::~TransferWidget() { delete ui; }

void TransferWidget::on_cancel_but_clicked() { emit closed(); }
