#include "creditwidget.h"
#include "ui_creditwidget.h"

CreditWidget::CreditWidget(IUser *user, QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditWidget), user(user) {
  ui->setupUi(this);
  bank_selector = std::make_unique<AddCardWidget>(this);

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

  validator = std::make_unique<QIntValidator>(10, 1000000);
  ui->amount_line->setValidator(validator.get());
}

void CreditWidget::show(LoanManager::LoanType mode) {
  current_mode = mode;
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

void CreditWidget::show_settings_widget() {
  bool visible = current_mode == LoanManager::LoanType::CREDIT;
  ui->percent_label->setVisible(visible);
  ui->percent_text_label->setVisible(visible);

  ui->stacked_widget->setCurrentIndex(Page::Settings);
  ui->percent_label->setText(QString::number(selected_bank->get_percent()));
}

void CreditWidget::update_credit() {
  size_t period_in_months = ui->period_chooser->currentText().toInt();
  if (ui->measure_chooser->currentIndex() == Measure::YEAR)
    period_in_months *= 12;
  switch (current_mode) {
  case LoanManager::LoanType::CREDIT:
    current_credit = std::make_unique<Credit>(
        user->get_id(), ui->amount_line->text().toULongLong(),
        selected_bank->get_percent(), period_in_months);
    break;
  case LoanManager::LoanType::INSTALLMENT:
    current_credit = std::make_unique<Installment>(
        user->get_id(), ui->amount_line->text().toULongLong(),
        period_in_months);
    break;
  }
}

void CreditWidget::show_info_widget() {
  ui->stacked_widget->setCurrentIndex(Page::Info);
  update_credit();
  ui->amount_label->setText(QString::number(current_credit->get_start_sum()));
  ui->pay_label->setText(QString::number(current_credit->get_payment()));
  ui->period_label->setText(ui->period_chooser->currentText() + " " +
                            measures[ui->measure_chooser->currentIndex()]);
}

void CreditWidget::on_measure_chooser_currentIndexChanged(int index) {
  update_labels(Measure(index));
}

void CreditWidget::cancel_but() { emit closed(); }

void CreditWidget::bank_selected(Bank *bank) {
  selected_bank = bank;
  show_settings_widget();
}

void CreditWidget::on_cancel_inf_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Settings);
}

void CreditWidget::on_confirm_inf_but_clicked() {
  qDebug() << (current_mode == LoanManager::LoanType::CREDIT ? "Credit"
                                                             : "Installment")
           << " request : "
           << LoanManager::loan_request(user, *current_credit, current_mode);
}

void CreditWidget::on_confirm_set_but_clicked() {
  QString amount = ui->amount_line->text();
  int index = 0;
  if (validator->validate(amount, index) == QValidator::Acceptable)
    show_info_widget();
}

void CreditWidget::on_cancel_set_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Select);
}
