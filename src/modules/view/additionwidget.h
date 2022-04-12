#ifndef ADDITIONWIDGET_H
#define ADDITIONWIDGET_H

#include "modules/entities/bank/accountaddmanager.h"
#include "periodwidget.h"

#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QWidget>

namespace Ui {
class AdditionWidget;
}

class AdditionWidget : public QWidget {
  Q_OBJECT

public:
  explicit AdditionWidget(QWidget *parent = nullptr);
  explicit AdditionWidget(Entity *initiator, QWidget *parent = nullptr);
  ~AdditionWidget();

  virtual void show(BankAccount *initiator);
  virtual void show();

signals:
  void closed();

private slots:
  void period_selected(size_t period);

  void on_cancel_but_clicked();
  void on_confirm_but_clicked();

private:
  enum Page { Settings, Period };

  // View
  Ui::AdditionWidget *ui;
  std::unique_ptr<PeriodWidget> period_widget;

  // Validators
  std::unique_ptr<QRegularExpressionValidator> card_validator;
  std::unique_ptr<QIntValidator> amount_validator;

  // Vars
  ISystemObject *sender;
  AccountAdd::Type type;

  size_t selected_period = 0;

  void init();
  void init_labels();
  void clear();
  void close();
  void warning(bool);
  bool input_is_corret();

  void send_deposit();
  void send_salary();
  void send_salary(size_t destination, size_t amount);

  static const inline int max_salary = 1000000;

  AdditionWidget(ISystemObject *initiator, AccountAdd::Type type,
                 QWidget *parent);
};

#endif // ADDITIONWIDGET_H
