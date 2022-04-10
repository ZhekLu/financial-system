#ifndef TRANSFERWIDGET_H
#define TRANSFERWIDGET_H

#include "modules/entities/bank/transactionmanager.h"
#include "modules/entities/bank/bankaccount.h"

#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QWidget>

namespace Ui {
class TransferWidget;
}

class TransferWidget : public QWidget {
  Q_OBJECT

public:
  enum Mode { Transfer, Withdraw, TopUp };
  explicit TransferWidget(IUser *user, QWidget *parent = nullptr);
  ~TransferWidget();

  void show(Mode, BankAccount *);

signals:
  void closed();

private slots:
  void on_cancel_but_clicked();

  void on_confirm_but_clicked();

private:
  Ui::TransferWidget *ui;
  IUser *user;
  BankAccount *account = nullptr;

  std::unique_ptr<QRegularExpressionValidator> card_validator;
  std::unique_ptr<QIntValidator> amount_validator;

  void show_warning(bool wrong_card_number);
  void hide_warning();
};

#endif // TRANSFERWIDGET_H
