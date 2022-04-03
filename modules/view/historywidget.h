#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include "modules/database/userdb.h"
#include "modules/entities/IUser.h"
#include "modules/entities/bank/creditmanager.h"
#include "modules/entities/bank/request.h"
#include "modules/entities/bank/transfermanager.h"
#include <QWidget>

namespace Ui {
class HistoryWidget;
}

class HistoryWidget : public QWidget {
  Q_OBJECT

public:
  explicit HistoryWidget(IUser *user, Request::Type request_type,
                         QWidget *parent = nullptr);
  ~HistoryWidget();

  bool mark(bool verified);
  bool selection_set() const { return current_index != -1; }

private slots:
  void update();
  void on_table_widget_cellClicked(int, int);

private:
  Ui::HistoryWidget *ui;
  Request::Type type;

  std::unique_ptr<IHistoryManager> manager;
  int current_index = -1;

  void update_grid();
};

#endif // HISTORYWIDGET_H
