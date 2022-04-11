#include "additionwidget.h"
#include "ui_additionwidget.h"

AdditionWidget::AdditionWidget(ISystemObject *initiator, AccountAdd::Type type,
                               QWidget *parent)
    : QWidget(parent), ui(new Ui::AdditionWidget), sender(initiator),
      type(type) {
  ui->setupUi(this);
  init();
}

AdditionWidget::AdditionWidget(QWidget *parent)
    : AdditionWidget(nullptr, AccountAdd::Type::DEPOSIT, parent) {}

AdditionWidget::AdditionWidget(Entity *initiator, QWidget *parent)
    : AdditionWidget(initiator, AccountAdd::Type::SALARY, parent) {}

AdditionWidget::~AdditionWidget() { delete ui; }

void AdditionWidget::show(BankAccount *initiator) {
  sender = initiator;
  show();
}

void AdditionWidget::show() {
  ui->stacked_widget->setCurrentIndex(Page::Period);
}

void AdditionWidget::period_selected(size_t period) {
  selected_period = period;
  if (type == AccountAdd::Type::SALARY)
    ui->stacked_widget->setCurrentIndex(Page::Settings);
  else
    on_confirm_but_clicked();
}

void AdditionWidget::init() {
  period_widget = std::make_unique<PeriodWidget>(this);
  connect(period_widget.get(), &PeriodWidget::selected, this,
          &AdditionWidget::period_selected);
  connect(period_widget.get(), &PeriodWidget::closed, this,
          &AdditionWidget::close);

  ui->stacked_widget->insertWidget(Page::Settings, ui->settings_widget);
  ui->stacked_widget->insertWidget(Page::Period, period_widget.get());

  init_labels();
}

void AdditionWidget::init_labels() {
  ui->warning_label->setText("incorrect card number");

  card_validator = std::make_unique<QRegularExpressionValidator>(
      QRegularExpression("^[0-9]{9}$"));
  ui->id_line->setValidator(card_validator.get());

  amount_validator = std::make_unique<QIntValidator>(10, max_salary);
  ui->amount_line->setValidator(amount_validator.get());

  warning(false);
}

void AdditionWidget::clear() {
  ui->amount_line->clear();
  ui->id_line->clear();
}

void AdditionWidget::close() {
  clear();
  emit closed();
}

void AdditionWidget::warning(bool warn) { ui->warning_label->setVisible(warn); }

void AdditionWidget::send_deposit() {

  BankAccount *initiator = (BankAccount *)sender;
  AccountAddManager::deposit_request(initiator->get_owner(),
                                     initiator->get_id(), selected_period);
}

void AdditionWidget::send_salary() {
  // check if data is valid
  QString card = ui->id_line->text();
  int pos = 0;
  if (card_validator->validate(card, pos) != QValidator::Acceptable) {
    warning(true);
    return;
  }

  // Get
  size_t amount = ui->amount_line->text().toULongLong();
  warning(false);
  size_t destination = ui->id_line->text().toULongLong();

  // Send
  Entity *initiator = (Entity *)sender;
  AccountAddManager::salary_request(initiator->get_id(), initiator->get_bank(),
                                    destination, selected_period, amount);
}

void AdditionWidget::on_cancel_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Period);
}

void AdditionWidget::on_confirm_but_clicked() {
  if (type == AccountAdd::SALARY)
    AdditionWidget::send_salary();
  else
    AdditionWidget::send_deposit();
  close();
}
