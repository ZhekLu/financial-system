#ifndef SYSTEMWINDOW_H
#define SYSTEMWINDOW_H

#include "historywidget.h"
#include <QMainWindow>

namespace Ui {
class SystemWindow;
}

class SystemWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit SystemWindow(QWidget *parent = nullptr);
  ~SystemWindow();

private:
  Ui::SystemWindow *ui;
  std::unique_ptr<HistoryWidget> history_widget;
  std::unique_ptr<HistoryWidget> credit_widget;
};

#endif // SYSTEMWINDOW_H
