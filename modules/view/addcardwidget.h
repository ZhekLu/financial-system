#ifndef ADDCARDWIDGET_H
#define ADDCARDWIDGET_H

#include "modules/entities/bank/accountmanager.h"
#include "modules/entities/bank/bankaccount.h"
#include <QWidget>

namespace Ui {
class AddCardWidget;
}

class AddCardWidget : public QWidget {
  Q_OBJECT

public:
  explicit AddCardWidget(QWidget *parent = nullptr);
  explicit AddCardWidget(Entity *company, QWidget *parent = nullptr);
  ~AddCardWidget();

signals:
  void closed();

private slots:
  void on_cance_but_clicked();

  void on_confirm_but_clicked();

private:
  Ui::AddCardWidget *ui;
  void send_add_account(size_t, size_t);
  void update_combobox();
};

#endif // ADDCARDWIDGET_H
