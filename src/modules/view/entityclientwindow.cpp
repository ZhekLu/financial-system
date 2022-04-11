#include "entityclientwindow.h"
#include "ui_entityclientwindow.h"

EntityClientWindow::EntityClientWindow(Entity *owner, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::EntityClientWindow), user(owner) {
  ui->setupUi(this);
  init();
}

EntityClientWindow::~EntityClientWindow() { delete ui; }

// Init

void EntityClientWindow::init() {
  salary_widget = std::make_unique<AdditionWidget>(user.get(), this);
  transfer_widget = std::make_unique<TransferWidget>(
      user.get(), TransferWidget::Type::Company, this);

  set_connections();

  ui->stacked_widget->insertWidget(WorkMode::SalaryView, salary_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::TransferView,
                                   transfer_widget.get());
}

void EntityClientWindow::set_connections() {
  connect(salary_widget.get(), &AdditionWidget::closed, this,
          &EntityClientWindow::mode_widget_closed);
  connect(transfer_widget.get(), &TransferWidget::closed, this,
          &EntityClientWindow::mode_widget_closed);
}

// Buttons

void EntityClientWindow::on_new_card_but_clicked() {
  qDebug() << "Add account : "
           << AccountManager::add_account_request(user->get_id(),
                                                  user->get_bank());
}

void EntityClientWindow::on_transfer_but_clicked() {}

void EntityClientWindow::on_salary_but_clicked() {
  salary_widget->show();
  ui->stacked_widget->setCurrentIndex(WorkMode::SalaryView);
}

void EntityClientWindow::on_info_but_clicked() {
  QMessageBox::about(this, "your info", user->get_info());
}

void EntityClientWindow::on_log_out_but_clicked() { this->close(); }

// Other

void EntityClientWindow::mode_widget_closed() {
  ui->stacked_widget->setCurrentIndex(WorkMode::CardView);
}
