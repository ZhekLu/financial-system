#ifndef ENTITYCLIENTWINDOW_H
#define ENTITYCLIENTWINDOW_H

#include "additionwidget.h"
#include "modules/entities/bank/accountmanager.h"
#include "modules/entities/entity.h"
#include "transferwidget.h"

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class EntityClientWindow;
}

class EntityClientWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit EntityClientWindow(Entity *owner, QWidget *parent = nullptr);
  ~EntityClientWindow();

private slots:
  void on_new_card_but_clicked();
  void on_transfer_but_clicked();
  void on_salary_but_clicked();
  void on_info_but_clicked();
  void on_log_out_but_clicked();

  void mode_widget_closed();

private:
  enum WorkMode { CardView, SalaryView, TransferView };
  // View
  Ui::EntityClientWindow *ui;

  // Mode widgets
  std::unique_ptr<AdditionWidget> salary_widget;
  std::unique_ptr<TransferWidget> transfer_widget;

  // Fields
  std::unique_ptr<Entity> user;

  void init();
  void set_connections();
};

#endif // ENTITYCLIENTWINDOW_H
