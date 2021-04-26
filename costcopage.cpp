#include "costcopage.h"

#include "ui_costcopage.h"

CostcoPage::CostcoPage(DbManager *db, QWidget *parent)
    : QDialog(parent), ui(new Ui::CostcoPage) {
  this->database = db;
  ui->setupUi(this);
}

CostcoPage::~CostcoPage() { delete ui; }

void CostcoPage::loadPurchasesTableFromDatabase() { return; }

void CostcoPage::loadMembersTableFromDatabase() {
  QList<QList<QString>> members = this->database->getMembers();
  qDebug() << members;
  return;
}

void CostcoPage::on_button_importPurchasesFromFileSelection_clicked() {
  this->database->importPurchasesFromFileSelection(this);
}

void CostcoPage::on_button_importMembersFromFileSelection_clicked() {
  this->database->importMembersFromFileSelection(this);
  loadMembersTableFromDatabase();
}
