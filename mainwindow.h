#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

public slots:
  void handle_login();
  void clear();

private:
  Ui::MainWindow *ui;
  QString usernameKey = "Costco+";
  QString passwordKey = "123456";
};
#endif // MAINWINDOW_H
