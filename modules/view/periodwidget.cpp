#include "periodwidget.h"
#include "ui_periodwidget.h"

PeriodWidget::PeriodWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PeriodWidget) {
  ui->setupUi(this);
  init_labels();
}

PeriodWidget::~PeriodWidget() { delete ui; }

void PeriodWidget::on_confirm_but_clicked() {
  size_t period_in_months = ui->period_chooser->currentText().toInt();
  if (ui->measure_chooser->currentIndex() == Measure::YEAR)
    period_in_months *= 12;
  emit selected(period_in_months);

  on_cance_but_clicked();
}

void PeriodWidget::on_cance_but_clicked() {
  update_labels(Measure::YEAR);
  emit closed();
}

void PeriodWidget::init_labels() {
  for (size_t i = 0; i < measures.size(); i++)
    ui->measure_chooser->addItem(measures[i]);

  ui->measure_chooser->setCurrentIndex(MONTH);
  update_labels(MONTH);
}

void PeriodWidget::update_labels(Measure measure) {
  ui->period_chooser->clear();
  for (size_t i = measure; i < periods.size(); i += 2) {
    ui->period_chooser->addItem(QString::number(periods[i]));
  }
}

void PeriodWidget::on_measure_chooser_currentIndexChanged(int index) {
  update_labels(Measure(index));
}
