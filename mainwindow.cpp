#include "mainwindow.h"
#include "costcopage.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::MainWindow(DbManager *db, QWidget *parent)
     : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->database = db;
  ui->setupUi(this);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handle_login() {

  if (this->ui->username->text() == usernameKey &&
      this->ui->password->text() == passwordKey) {
    this->clear();
    this->ui->error->setText("");
    CostcoPage page{this->database};
    page.setModal(true);
    page.exec();
  } else {
    this->ui->error->setText("Incorrect login information!!!");
    this->clear();
  }
}

void MainWindow::clear() {
  this->ui->username->setText("");
  this->ui->password->setText("");
}
