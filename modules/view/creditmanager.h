#ifndef CREDIT_WIDGETH_H
#define CREDIT_WIDGETH_H

#include "modules/entities/bank/bankaccount.h"
#include <QIntValidator>
#include <QWidget>
#include <memory>
#include <vector>

namespace Ui {
class CreditManager;
}

class CreditManager : public QWidget {
  Q_OBJECT

public:
  enum Measure { MONTH, YEAR };

  explicit CreditManager(QWidget *parent = nullptr);
  ~CreditManager();

  void show(BankAccount *account);

private slots:
  void on_confirm_but_clicked();

  void on_cance_but_clicked();

  void on_measure_chooser_currentIndexChanged(int index);

private:
  Ui::CreditManager *ui;
  BankAccount *account;

  std::unique_ptr<QIntValidator> validator;

  std::vector<int> periods;
  std::vector<QString> measures;

  void init_labels();
  void update_labels(Measure);
};

#endif // CREDIT_WIDGETH_H
