#include "modules/mainwindow.h"

#include <QApplication>
#include <ctime>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  srand(static_cast<unsigned int>(time(NULL)));
  //  for (int i = 0; i < 10; i++) {
  //    qDebug() << rand();
  //  }
  //  //  const unsigned int rand_init = 0;
  MainWindow w;
  w.show();
  return a.exec();
}
