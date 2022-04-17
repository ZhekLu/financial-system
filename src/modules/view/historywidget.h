#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include "modules/database/userdb.h"
#include "modules/entities/IUser.h"
#include "modules/entities/bank/accountaddmanager.h"
#include "modules/entities/bank/accountstatemanager.h"
#include "modules/entities/bank/loanmanager.h"
#include "modules/entities/bank/loginmanager.h"
#include "modules/entities/bank/request.h"
#include "modules/entities/bank/transactionmanager.h"
#include <QWidget>

namespace Ui {
class HistoryWidget;
}

class HistoryWidget : public QWidget {
  Q_OBJECT

public:
  explicit HistoryWidget(IUser *user, Request::Type request_type,
                         IHistoryManager::ItemsType user_mode,
                         QWidget *parent = nullptr);
  ~HistoryWidget();

  bool mark(bool verified);
  bool selection_set() const { return current_index != -1; }
  size_t get_selected() const { return manager->get_selected(current_index); }

private slots:
  void update();
  void on_table_widget_cellClicked(int, int);

private:
  Ui::HistoryWidget *ui;
  Request::Type type;
  IHistoryManager::ItemsType user_mode;

  std::unique_ptr<IHistoryManager> manager;
  int current_index = -1;

  void update_grid();
  void manager_factory(IUser *user, Request::Type request_type);
};

#endif // HISTORYWIDGET_H
