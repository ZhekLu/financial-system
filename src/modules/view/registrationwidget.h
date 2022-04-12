#ifndef REGISTRATIONWIDGET_H
#define REGISTRATIONWIDGET_H

#include "authwidget.h"
#include <QWidget>

namespace Ui {
class RegistrationWidget;
class LoginWidget;
} // namespace Ui

class RegistrationWidget : public QWidget {
  Q_OBJECT

public:
  explicit RegistrationWidget(QWidget *parent = nullptr);
  ~RegistrationWidget();

  // methods
  virtual void show();
  virtual void hide();

private slots:
  void on_confirm_but_clicked();
  void on_cancel_but_clicked();

  // checkers
  void check_email();
  void check_bic();
  void check_phone();
  void check_passport();
  void check_passport_id();

private:
  enum Page { InfoPage, LoginPage };

  Ui::RegistrationWidget *ui;
  Ui::LoginWidget *lw;
  std::unique_ptr<QWidget> login_widget;

  std::unique_ptr<QRegularExpressionValidator> phone_validator;
  std::unique_ptr<QRegularExpressionValidator> email_validator;
  std::unique_ptr<QRegularExpressionValidator> bic_validator;
  std::unique_ptr<QRegularExpressionValidator> passport_validator;
  std::unique_ptr<QRegularExpressionValidator> passport_id_validator;

  void init();
  void init_lines();
  void set_connections();

  bool move_page(bool back = false);
  void clear_all();
  void warning(bool visible);

  bool is_valid(QLineEdit *line);
  bool send_reg_request();

  void update_style(QLineEdit *line);
  void set_page(Page page);
  void set_buttons(Page page);
};

#endif // REGISTRATIONWIDGET_H
