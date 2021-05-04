#include "costcopage.h"

#include "ui_costcopage.h"

CostcoPage::CostcoPage(DbManager *db, QWidget *parent)
  : QDialog(parent), ui(new Ui::CostcoPage) {
  this->database = db;
  ui->setupUi(this);
  this->loadMembersTableFromDatabase();
  this->loadPurchasesTableFromDatabase();
}

CostcoPage::~CostcoPage() { delete ui; }

void CostcoPage::loadPurchasesTableFromDatabase() {
  const int membersColumnsToRead = 5;
  QTableWidget *table = this->ui->SalesReportTable;
  QList<QList<QString>> members = this->database->getPurchases();
  for (int i = 0; i < members.count(); i++) {
      table->insertRow(i);
      for (int z = 0; z < membersColumnsToRead; z++) {
          QTableWidgetItem *tableItem = new QTableWidgetItem;
          tableItem->setText(members.at(i).at(z));
          table->setItem(i, z, tableItem);
        }
    }
  return;
}

void CostcoPage::loadMembersTableFromDatabase() {
  const int membersColumnsToRead = 4;
  QTableWidget *table = this->ui->MembershipInformationTable;
  QList<QList<QString>> members = this->database->getMembers();
  for (int i = 0; i < members.count(); i++) {
      table->insertRow(i);
      for (int z = 0; z < membersColumnsToRead; z++) {
          QTableWidgetItem *tableItem = new QTableWidgetItem;
          tableItem->setText(members.at(i).at(z));
          table->setItem(i, z, tableItem);
        }
    }
  return;
}

void CostcoPage::on_button_importPurchasesFromFileSelection_clicked() {
  this->database->importPurchasesFromFileSelection(this);
  loadPurchasesTableFromDatabase();
}

void CostcoPage::on_button_importMembersFromFileSelection_clicked() {
  this->database->importMembersFromFileSelection(this);
  loadMembersTableFromDatabase();
}
