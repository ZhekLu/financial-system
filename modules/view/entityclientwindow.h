#ifndef ENTITYCLIENTWINDOW_H
#define ENTITYCLIENTWINDOW_H

#include "modules/entities/entity.h"
#include <QMainWindow>

namespace Ui {
class EntityClientWindow;
}

class EntityClientWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit EntityClientWindow(/*Entity *owner,*/ QWidget *parent = nullptr);
  ~EntityClientWindow();

private:
  Ui::EntityClientWindow *ui;

  std::unique_ptr<IUser> user;
};

#endif // ENTITYCLIENTWINDOW_H
