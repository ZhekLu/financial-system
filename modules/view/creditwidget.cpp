#include "creditwidget.h"
#include "ui_creditwidget.h"

CreditWidget::CreditWidget(
    IUser *user, std::unordered_map<size_t, std::unique_ptr<Bank>> &banks_list,
    QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditWidget), user(user) {
  ui->setupUi(this);
  bank_selector = std::make_unique<AddCardWidget>(banks_list, this);

  init_labels();
  connect_pages();
}

CreditWidget::~CreditWidget() { delete ui; }

void CreditWidget::connect_pages() {
  ui->stacked_widget->insertWidget(Page::Settings, ui->settings_widget);
  ui->stacked_widget->insertWidget(Page::Select, bank_selector.get());

  connect(bank_selector.get(), &AddCardWidget::closed, this,
          &CreditWidget::cancel_but);
  connect(bank_selector.get(), &AddCardWidget::selected, this,
          &CreditWidget::bank_selected);
}

void CreditWidget::init_labels() {

  measures = {"Month", "Year"};
  for (size_t i = 0; i < measures.size(); i++) {
    ui->measure_chooser->addItem(measures[i]);
  }
  ui->measure_chooser->setCurrentIndex(MONTH);

  // i % 2 == 1 - month, 0 - year
  periods = {3, 1, 6, 2, 12, 5, 24, 10};
  update_labels(MONTH);

  validator = std::make_unique<QIntValidator>();
  ui->amount_line->setValidator(validator.get());
}

void CreditWidget::show() {
  ui->stacked_widget->setCurrentIndex(Page::Select);
  bank_selector->show();
  ui->amount_line->clear();
}

void CreditWidget::update_labels(Measure measure) {
  ui->period_chooser->clear();
  for (size_t i = measure; i < periods.size(); i += 2) {
    ui->period_chooser->addItem(QString::number(periods[i]));
  }
}

void CreditWidget::show_settings_widget() {}

void CreditWidget::on_measure_chooser_currentIndexChanged(int index) {
  update_labels(Measure(index));
}

void CreditWidget::cancel_but() { emit closed(); }

void CreditWidget::bank_selected(Bank *bank) {
  selected_bank = bank;
  show_settings_widget();
  ui->stacked_widget->setCurrentIndex(Page::Settings);
}

void CreditWidget::on_cancel_inf_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Settings);
}

void CreditWidget::on_confirm_inf_but_clicked() {}

void CreditWidget::on_confirm_set_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Info);
}

void CreditWidget::on_cancel_set_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Select);
}
