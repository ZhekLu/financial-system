#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(IUser *owner, AccessMode mode, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClientWindow), user(owner), mode(mode),
      current_account(nullptr) {
  ui->setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);

  init();
}

ClientWindow::~ClientWindow() { delete ui; }

void ClientWindow::clear_selected() {
  current_account = nullptr;
  card_widget->clear_selection();
  ui->current_label->clear();
}

void ClientWindow::update_buttons() {
  bool vis = !current_account->is_frozen() && !current_account->is_blocked();

  ui->freeze_but->setText(current_account->is_frozen() ? "Unfreeze" : "Freeze");
  ui->block_but->setText(current_account->is_blocked() ? "Unblock" : "Block");

  ui->transfer_but->setEnabled(vis);
  ui->withdraw_but->setEnabled(vis);
}

// Init methods

void ClientWindow::init() {
  init_widgets();
  set_connections();
  init_stack();
}

void ClientWindow::init_widgets() {
  card_widget = std::make_unique<AccountsWidget>(user.get(), this);
  add_widget = std::make_unique<AddCardWidget>(user.get(), banks, this);
  credit_widget = std::make_unique<CreditWidget>(user.get(), banks, this);
  transfer_widget = std::make_unique<TransferWidget>(
      user.get(), TransferWidget::Type::User, this);
  deposit_widget = std::make_unique<AdditionWidget>(this);
  manage_window = std::make_unique<ClientManageWindow>(user.get(), this);
}

void ClientWindow::init_stack() {
  ui->stacked_widget->insertWidget(WorkMode::CardView, card_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::CreditView, credit_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::AddCardView, add_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::TransferView,
                                   transfer_widget.get());
  ui->stacked_widget->insertWidget(WorkMode::DepositView, deposit_widget.get());
}

void ClientWindow::set_connections() {
  connect(card_widget.get(), &AccountsWidget::selected, this,
          &ClientWindow::account_selected);
  // close events
  connect(credit_widget.get(), &CreditWidget::closed, this,
          &ClientWindow::mode_widget_closed);
  connect(add_widget.get(), &AddCardWidget::closed, this,
          &ClientWindow::mode_widget_closed);
  connect(transfer_widget.get(), &TransferWidget::closed, this,
          &ClientWindow::mode_widget_closed);
  connect(deposit_widget.get(), &AdditionWidget::closed, this,
          &ClientWindow::mode_widget_closed);
  connect(manage_window.get(), &ClientManageWindow::closed, this,
          &ClientWindow::manage_window_closed);
}

// Buttons slots

void ClientWindow::on_log_out_but_clicked() { this->close(); }

void ClientWindow::on_info_but_clicked() {
  QMessageBox::about(this, "your info", user->get_info());
}

void ClientWindow::on_freeze_but_clicked() {
  if (current_account)
    qDebug() << ui->freeze_but->text()
             << AccountStateManager::freeze_request(
                    user->get_id(), current_account->get_id(),
                    !current_account->is_frozen());
}

void ClientWindow::on_block_but_clicked() {
  if (current_account)
    qDebug() << ui->block_but->text()
             << AccountStateManager::block_request(
                    user->get_id(), current_account->get_id(),
                    !current_account->is_blocked());
}

void ClientWindow::on_withdraw_but_clicked() {
  if (!current_account)
    return;
  transfer_widget->show(TransferWidget::Mode::Withdraw, current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::TransferView);
}

void ClientWindow::on_new_card_but_clicked() {
  clear_selected();
  add_widget->show();
  ui->stacked_widget->setCurrentIndex(WorkMode::AddCardView);
}

void ClientWindow::on_transfer_but_clicked() {
  if (!current_account)
    return;
  transfer_widget->show(TransferWidget::Mode::Transfer, current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::TransferView);
}

void ClientWindow::on_credit_but_clicked() {
  clear_selected();
  credit_widget->show(LoanManager::CREDIT);
  ui->stacked_widget->setCurrentIndex(WorkMode::CreditView);
}

void ClientWindow::on_installment_but_clicked() {
  clear_selected();
  credit_widget->show(LoanManager::INSTALLMENT);
  ui->stacked_widget->setCurrentIndex(WorkMode::CreditView);
}

void ClientWindow::on_deposit_but_clicked() {
  if (!current_account)
    return;
  deposit_widget->show(current_account);
  ui->stacked_widget->setCurrentIndex(WorkMode::DepositView);
}

void ClientWindow::on_manage_but_clicked() {
  manage_window->show();
  this->hide();
}

// Upd slots

void ClientWindow::account_selected(BankAccount *selected) {
  current_account = selected;

  // upd view
  if (current_account)
    update_buttons();

  ui->current_label->setText(
      current_account
          ? "Selected account : " + QString::number(current_account->get_id())
          : "");
}

void ClientWindow::mode_widget_closed() {
  ui->stacked_widget->setCurrentIndex(WorkMode::CardView);
}

void ClientWindow::manage_window_closed() {
  this->show();
  manage_window->hide();
}
