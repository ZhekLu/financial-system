#ifndef TRANSFERWIDGET_H
#define TRANSFERWIDGET_H

#include "modules/entities/bank/bankaccount.h"
#include "modules/entities/bank/transactionmanager.h"

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
  enum Type { User, Company };

  //    enum Type
  explicit TransferWidget(IUser *user, Type type, QWidget *parent = nullptr);
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
  Type type;

  BankAccount *account = nullptr;

  std::unique_ptr<QRegularExpressionValidator> card_validator;
  std::unique_ptr<QIntValidator> amount_validator;

  void show_warning(bool wrong_card_number);
  void hide_warning();

  void send_transaction(size_t destination, size_t amount);
  void send_withdraw(size_t amount);
};

#endif // TRANSFERWIDGET_H
