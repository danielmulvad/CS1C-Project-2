#include <database.h>

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  DbManager database{QString{QDir::currentPath() + "costco+.db"}};
  w.show();
  return a.exec();
}
