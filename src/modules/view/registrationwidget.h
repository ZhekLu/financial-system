#ifndef REGISTRATIONWIDGET_H
#define REGISTRATIONWIDGET_H

#include "authwidget.h"
#include <QWidget>

namespace Ui {
class RegistrationWidget;
}

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

private:
  enum Page { InfoPage, LoginPage };

  Ui::RegistrationWidget *ui;
  std::unique_ptr<AuthWidget> aw;

  std::unique_ptr<QRegularExpressionValidator> phone_validator;
  std::unique_ptr<QRegularExpressionValidator> email_validator;
  std::unique_ptr<QRegularExpressionValidator> bic_validator;

  void init();
  void init_lines();

  void check_input();
  bool move_page(bool back = false);
  void clear_all();

  void set_style(QLineEdit *line, QValidator *validator);
  void set_page(Page page);
  void set_buttons(Page page);
};

#endif // REGISTRATIONWIDGET_H
