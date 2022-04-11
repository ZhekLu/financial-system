#ifndef PERIODWIDGET_H
#define PERIODWIDGET_H

#include <QWidget>

namespace Ui {
class PeriodWidget;
}

class PeriodWidget : public QWidget {
  Q_OBJECT

public:
  enum Measure { MONTH, YEAR };

  explicit PeriodWidget(QWidget *parent = nullptr);
  ~PeriodWidget();

signals:
  void closed();
  void selected(size_t period_in_month);

private slots:
  void on_confirm_but_clicked();
  void on_cance_but_clicked();

  void on_measure_chooser_currentIndexChanged(int index);

private:
  Ui::PeriodWidget *ui;

  // view methods
  void init_labels();
  void update_labels(Measure);

  // i % 2 == 1 - month, 0 - year
  static inline std::vector<size_t> periods = {3, 1, 6, 2, 12, 5, 24, 10};
  static inline std::vector<QString> measures = {"Month", "Year"};
};

#endif // PERIODWIDGET_H
