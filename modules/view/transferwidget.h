#ifndef TRANSFERWIDGET_H
#define TRANSFERWIDGET_H

#include <QWidget>

namespace Ui {
class TransferWidget;
}

class TransferWidget : public QWidget {
  Q_OBJECT

public:
  explicit TransferWidget(QWidget *parent = nullptr);
  ~TransferWidget();

signals:
  void closed();

private slots:
  void on_cancel_but_clicked();

private:
  Ui::TransferWidget *ui;
};

#endif // TRANSFERWIDGET_H
