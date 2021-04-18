#include "mainwindow.h"

#include <QApplication>
#include <database.h>
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  DbManager database {"costco"};
  w.show();
  return a.exec();
}
