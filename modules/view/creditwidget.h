#ifndef CREDITWIDGET_H
#define CREDITWIDGET_H

#include "modules/entities/bank/bank.h"
#include "modules/entities/bank/bankaccount.h"
#include <QIntValidator>
#include <QWidget>
#include <memory>
#include <vector>

namespace Ui {
class CreditWidget;
}

class CreditWidget : public QWidget {
  Q_OBJECT

public:
  enum Measure { MONTH, YEAR };

  explicit CreditWidget(QWidget *parent = nullptr);
  ~CreditWidget();

  void show(BankAccount *account);

signals:
  void closed();

private slots:
  void on_confirm_but_clicked();

  void on_cance_but_clicked();

  void on_measure_chooser_currentIndexChanged(int index);

  void on_cancel_inf_but_clicked();

  void on_confirm_inf_but_clicked();

private:
  enum Page { Info, Settings };

  Ui::CreditWidget *ui;
  BankAccount *account;
  std::unique_ptr<Bank> bank;

  std::unique_ptr<QIntValidator> validator;

  std::vector<int> periods;
  std::vector<QString> measures;

  void init_labels();
  void update_labels(Measure);
};

#endif // CREDITWIDGET_H
