#include "mainwindow.h"
#include "dashboard.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  database = new DbManager{QString{QDir::currentPath() + "/costco+.db"}};
  ui->setupUi(this);
}

MainWindow::MainWindow(DbManager *db, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  this->database = db;
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
  delete database;
}

void MainWindow::handle_login() {
  if (this->ui->username->text() == usernameKey &&
      this->ui->password->text() == passwordKey) {
    this->clear();
    this->ui->error->setText("");
    this->dashboard = new Dashboard{this->database};
    dashboard->show();
    this->close();
  } else {
    this->ui->error->setText("Incorrect login information!!!");
    this->clear();
  }
}

void MainWindow::clear() {
  this->ui->username->setText("");
  this->ui->password->setText("");
}
