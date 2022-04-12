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

  // Next step
  if (type == AccountAdd::Type::SALARY) {
    ui->stacked_widget->setCurrentIndex(Page::Settings);
    return;
  }

  send_deposit();
  close();
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

bool AdditionWidget::input_is_corret() {
  QString card = ui->id_line->text();
  int pos = 0;
  if (card_validator->validate(card, pos) != QValidator::Acceptable) {
    warning(true);
    return false;
  }
  QString amount = ui->amount_line->text();
  pos = 0;
  if (amount_validator->validate(amount, pos) != QValidator::Acceptable) {
    warning(true);
    return false;
  }
  warning(false);
  return true;
}

void AdditionWidget::send_deposit() {

  BankAccount *initiator = (BankAccount *)sender;
  qDebug() << "Deposit request : "
           << AccountAddManager::deposit_request(
                  initiator->get_owner(), initiator->get_id(), selected_period);
}

void AdditionWidget::send_salary() {
  // Get
  size_t amount = ui->amount_line->text().toULongLong();
  size_t destination = ui->id_line->text().toULongLong();

  // Send
  send_salary(destination, amount);
}

void AdditionWidget::send_salary(size_t destination, size_t amount) {
  Entity *initiator = (Entity *)sender;
  qDebug() << "Salary request : "
           << AccountAddManager::salary_request(
                  initiator->get_id(), initiator->get_bank(), destination,
                  selected_period, amount);
}

void AdditionWidget::on_cancel_but_clicked() {
  ui->stacked_widget->setCurrentIndex(Page::Period);
}

void AdditionWidget::on_confirm_but_clicked() {
  if (!input_is_corret())
    return;
  AdditionWidget::send_salary();
  close();
}
