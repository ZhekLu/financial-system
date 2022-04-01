#include "creditwidget.h"
#include "ui_creditwidget.h"

CreditWidget::CreditWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditWidget), account(nullptr) {
  ui->setupUi(this);
  validator = std::make_unique<QIntValidator>();
  init_labels();

  ui->stacked_widget->insertWidget(Page::Settings, ui->settings_widget);
  ui->stacked_widget->setCurrentIndex(Page::Settings);
}

CreditWidget::~CreditWidget() { delete ui; }

void CreditWidget::show(BankAccount *current_account) {
  //  SYS

  account = current_account;
}

void CreditWidget::on_confirm_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Info);
}

void CreditWidget::on_cance_but_clicked() { emit closed(); }

void CreditWidget::init_labels() {

  measures = {"Month", "Year"};
  for (size_t i = 0; i < measures.size(); i++) {
    ui->measure_chooser->addItem(measures[i]);
  }
  ui->measure_chooser->setCurrentIndex(MONTH);

  // i % 2 == 1 - month, 0 - year
  periods = {3, 1, 6, 2, 12, 5, 24, 10};
  update_labels(MONTH);
  ui->amount_line->setValidator(validator.get());
}

void CreditWidget::update_labels(Measure measure) {
  ui->period_chooser->clear();
  for (size_t i = measure; i < periods.size(); i += 2) {
    ui->period_chooser->addItem(QString::number(periods[i]));
  }
}

void CreditWidget::on_measure_chooser_currentIndexChanged(int index) {
  update_labels(Measure(index));
}

void CreditWidget::on_cancel_inf_but_clicked() { emit closed(); }

void CreditWidget::on_confirm_inf_but_clicked() {}
