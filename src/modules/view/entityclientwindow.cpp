#include "entityclientwindow.h"
#include "ui_entityclientwindow.h"

EntityClientWindow::EntityClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EntityClientWindow)
{
    ui->setupUi(this);
}

EntityClientWindow::~EntityClientWindow()
{
    delete ui;
}
