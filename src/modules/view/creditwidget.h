#ifndef CREDITWIDGET_H
#define CREDITWIDGET_H

#include "addcardwidget.h"
#include "modules/entities/bank/loanmanager.h"
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

  explicit CreditWidget(IUser *user, QWidget *parent = nullptr);
  ~CreditWidget();

  void show(LoanManager::LoanType mode);

signals:
  void closed();

private slots:
  void cancel_but();
  void on_confirm_set_but_clicked();
  void on_cancel_set_but_clicked();
  void on_cancel_inf_but_clicked();
  void on_confirm_inf_but_clicked();
  void on_measure_chooser_currentIndexChanged(int index);
  void bank_selected(Bank *bank);

private:
  enum Page { Info, Settings, Select };

  Ui::CreditWidget *ui;
  std::unique_ptr<AddCardWidget> bank_selector;

  IUser *user;
  //  Mode current_mode;
  LoanManager::LoanType current_mode;

  Bank *selected_bank;
  std::unique_ptr<Loan> current_credit;

  std::unique_ptr<QIntValidator> validator;

  std::vector<int> periods;
  std::vector<QString> measures;

  void connect_pages();
  void init_labels();
  void update_labels(Measure);
  void show_settings_widget();

  void update_credit();
  void show_info_widget();
};

#endif // CREDITWIDGET_H
