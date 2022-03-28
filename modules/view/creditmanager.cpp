#include "creditmanager.h"
#include "ui_creditmanager.h"

CreditManager::CreditManager(QWidget *parent)
    : QWidget(parent), ui(new Ui::CreditManager), account(nullptr) {
  ui->setupUi(this);
  validator = std::make_unique<QIntValidator>();
  init_labels();
}

CreditManager::~CreditManager() { delete ui; }

void CreditManager::show(BankAccount *current_account) {
  //  SYS

  QWidget::show();
  account = current_account;
}

void CreditManager::on_confirm_but_clicked() {}

void CreditManager::on_cance_but_clicked() { this->hide(); }

void CreditManager::init_labels() {

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

void CreditManager::update_labels(Measure measure) {
  ui->period_chooser->clear();
  for (size_t i = measure; i < periods.size(); i += 2) {
    ui->period_chooser->addItem(QString::number(periods[i]));
  }
}

void CreditManager::on_measure_chooser_currentIndexChanged(int index) {
  update_labels(Measure(index));
}
