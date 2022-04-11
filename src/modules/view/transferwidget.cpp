#include "transferwidget.h"
#include "ui_transferwidget.h"

TransferWidget::TransferWidget(IUser *user, Type type, QWidget *parent)
    : QWidget(parent), ui(new Ui::TransferWidget), user(user), type(type) {
  ui->setupUi(this);

  card_validator = std::make_unique<QRegularExpressionValidator>(
      QRegularExpression("^[0-9]{9}$"));
  ui->id_line->setValidator(card_validator.get());

  amount_validator = std::make_unique<QIntValidator>(0, 0);
  ui->amount_line->setValidator(amount_validator.get());

  hide_warning();
}

TransferWidget::~TransferWidget() { delete ui; }

void TransferWidget::show(Mode mode, BankAccount *acc) {
  if (!acc)
    return;
  account = acc;
  bool visible = mode == Transfer;
  ui->id_label->setVisible(visible);
  ui->id_line->setVisible(visible);

  amount_validator->setTop(account->get_balance());
}

void TransferWidget::on_cancel_but_clicked() {
  emit closed();
  ui->id_line->clear();
  ui->amount_line->clear();
  hide_warning();
}

void TransferWidget::on_confirm_but_clicked() {

  // check if data is valid

  if (ui->id_label->isVisible()) {
    QString card = ui->id_line->text();
    int pos = 0;
    if (card_validator->validate(card, pos) != QValidator::Acceptable) {
      show_warning(true);
      return;
    }
  }

  size_t amount = ui->amount_line->text().toULongLong();
  if (!account->can_pay(amount)) {
    show_warning(false);
    return;
  }

  hide_warning();
  if (!amount)
    on_cancel_but_clicked();

  if (ui->id_label->isVisible()) {
    size_t destination = ui->id_line->text().toULongLong();
    if (destination != account->get_id())
      send_transaction(destination, amount);
  } else {
    send_withdraw(amount);
  }
  on_cancel_but_clicked();
}

void TransferWidget::send_transaction(size_t destination, size_t amount) {
  qDebug() << "Transaction:"
           << (type == Type::User
                   ? TransactionManager::transfer_request(
                         user->get_id(), account->get_id(), destination, amount)
                   : TransactionManager::entity_transfer_request(
                         user->get_id(), account->get_id(), destination,
                         amount));
}

void TransferWidget::send_withdraw(size_t amount) {
  qDebug() << "Withdraw"
           << TransactionManager::withdraw_request(user->get_id(),
                                                   account->get_id(), amount);
}

void TransferWidget::show_warning(bool wrong_card) {
  ui->warning_label->setVisible(true);
  ui->warning_label->setText(wrong_card ? "incorrect card number"
                                        : "u don't have enought money");
}

void TransferWidget::hide_warning() { ui->warning_label->setVisible(false); }
