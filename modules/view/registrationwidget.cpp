#include "registrationwidget.h"
#include "ui_registrationwidget.h"

RegistrationWidget::RegistrationWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegistrationWidget) {
  ui->setupUi(this);
  init();
}

RegistrationWidget::~RegistrationWidget() { delete ui; }

void RegistrationWidget::show() {
  QWidget::show();
  set_page(Page::InfoPage);
}

void RegistrationWidget::hide() {
  QWidget::hide();
  clear_all();
}

void RegistrationWidget::init() {
  aw = std::make_unique<AuthWidget>(this);

  ui->stacked_widget->insertWidget(Page::InfoPage, ui->tab_widget);
  ui->stacked_widget->insertWidget(Page::LoginPage, aw.get());
  ui->stacked_widget->setCurrentIndex(Page::InfoPage);

  init_lines();
}

void RegistrationWidget::init_lines() {
  QRegularExpression mail("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                          QRegularExpression::CaseInsensitiveOption);
  email_validator = std::make_unique<QRegularExpressionValidator>(mail, this);
  //  ui->email_line->setValidator(new QRegularExpressionValidator(mail, this));
  ui->email_line->setValidator(email_validator.get());

  QRegularExpression phone("^\\+375(44|25|33)[0-9]{7}$");
  phone_validator = std::make_unique<QRegularExpressionValidator>(phone, this);
  //  QRegularExpression phone("^[A-Z]{4}\\-[0-9]{4}\\-[0-9]{4}$");
  //  ui->phone_line->setValidator(new QRegularExpressionValidator(phone,
  //  this));
  ui->phone_line->setValidator(phone_validator.get());

  QRegularExpression bic("^[1-9]{1}[0-9]{8}$");
  bic_validator = std::make_unique<QRegularExpressionValidator>(bic, this);
  ui->bic_line->setValidator(bic_validator.get());
  ui->bank_bic_line->setValidator(bic_validator.get());
  // Connect
  QObject::connect(ui->email_line, &QLineEdit::textChanged, this,
                   &RegistrationWidget::check_input);
  QObject::connect(ui->phone_line, &QLineEdit::textChanged, this,
                   &RegistrationWidget::check_input);
  QObject::connect(ui->bic_line, &QLineEdit::textChanged, this,
                   &RegistrationWidget::check_input);
  QObject::connect(ui->bank_bic_line, &QLineEdit::textChanged, this,
                   &RegistrationWidget::check_input);
}

void RegistrationWidget::check_input() {
  //  QString amount = ui->phone_line->text();
  //  int index = 0;
  //  if (phone_validator->validate(amount, index) == QValidator::Acceptable)
  //    ui->warning_label->setText("ok");
  //  else
  //    ui->warning_label->setText("no");
  set_style(ui->email_line, email_validator.get());
  set_style(ui->phone_line, phone_validator.get());
  set_style(ui->bank_bic_line, bic_validator.get());
  set_style(ui->bic_line, bic_validator.get());
}

bool RegistrationWidget::move_page(bool back) {
  if (ui->stacked_widget->currentIndex() == Page::LoginPage) {
    if (back)
      set_page(Page::InfoPage);
    return back;
  }
  if (!back)
    set_page(Page::LoginPage);
  return !back;
}

void RegistrationWidget::clear_all() {
  // Individual
  ui->i_name_line->clear();
  ui->passport_line->clear();
  ui->id_line->clear();
  ui->email_line->clear();
  ui->adress_line->clear();
  // Entity
  ui->e_name_line->clear();
  ui->pac_line->clear();
  ui->bic_line->clear();
  ui->adress_line->clear();
  ui->bank_bic_line->clear();
}

void RegistrationWidget::set_style(QLineEdit *line, QValidator *validator) {
  QString text = ui->phone_line->text();
  int index = 0;
  if (validator->validate(text, index) == QValidator::Acceptable)
    line->setStyleSheet(
        "background-color: rgb(173, 173, 173);color: rgb(0, 0, 0);");
  else
    line->setStyleSheet(
        "background-color: rgb(173, 92, 92);color: rgb(0, 0, 0);");
}

void RegistrationWidget::set_page(Page page) {
  ui->stacked_widget->setCurrentIndex(page);
  set_buttons(page);
}

void RegistrationWidget::set_buttons(Page page) {
  switch (page) {
  case RegistrationWidget::InfoPage:
    ui->cancel_but->setText("Cancel");
    ui->confirm_but->setText("Continue");
    break;
  case RegistrationWidget::LoginPage:
    ui->cancel_but->setText("Back");
    ui->confirm_but->setText("Confirm");
    break;
  }
}

void RegistrationWidget::on_confirm_but_clicked() {
  if (move_page())
    return;
}

void RegistrationWidget::on_cancel_but_clicked() {
  if (!move_page(true))
    this->hide();
}
