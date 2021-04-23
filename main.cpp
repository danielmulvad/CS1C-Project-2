#include <database.h>

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  DbManager database{QString{QDir::currentPath() + "costco+.db"}};
  QApplication a(argc, argv);
  MainWindow w{&database};
  w.show();
  return a.exec();
}
