#ifndef ADDCARDWIDGET_H
#define ADDCARDWIDGET_H

#include "modules/entities/bank/accountstatemanager.h"
#include "modules/entities/bank/bank.h"
#include "modules/entities/bank/bankaccount.h"

#include <QWidget>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Ui {
class AddCardWidget;
}

class AddCardWidget : public QWidget {
  Q_OBJECT

public:
  explicit AddCardWidget(IUser *user, QWidget *parent = nullptr);
  explicit AddCardWidget(QWidget *parent = nullptr);
  ~AddCardWidget();

  void show();

signals:
  void closed();
  void selected(Bank *);

private slots:
  void on_cance_but_clicked();
  void on_confirm_but_clicked();

private:
  enum Mode { BankSelector, CardAdder };

  Ui::AddCardWidget *ui;

  IUser *user;
  std::vector<size_t> banks_indexes;
  Mode mode;

  static inline std::unordered_map<size_t, std::unique_ptr<Bank>> banks =
      USER_DB->get_hash_banks();
  AddCardWidget(Mode mode, IUser *user, QWidget *parent);

  void update();
  void send_add_account(size_t, size_t);
  void update_combobox();
  void company_mode();
  void person_mode();
};

#endif // ADDCARDWIDGET_H
