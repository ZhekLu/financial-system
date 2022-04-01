#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "addcardwidget.h"
#include "transferwidget.h"
#include <QMainWindow>

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit TestWindow(QWidget *parent = nullptr);
  ~TestWindow();

private slots:
  void on_pushButton_clicked();

  void on_pushButton_2_clicked();

private:
  Ui::TestWindow *ui;
  TransferWidget *tf;
  AddCardWidget *aw;
};

#endif // TESTWINDOW_H
