#include "dashboard.h"
#include "ui_dashboard.h"

Dashboard::Dashboard(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Dashboard) {
  ui->setupUi(this);
}

Dashboard::Dashboard(DbManager *db, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Dashboard) {
  this->database = db;
  ui->setupUi(this);
  this->loadMembersTableFromDatabase();
  this->loadPurchasesTableFromDatabase();
  this->loadInventoryTableFromDatabase();
}

Dashboard::~Dashboard() { delete ui; }

void Dashboard::loadPurchasesTableFromDatabase() {
  const int purchasesColumnsToRead = 5;
  QTableWidget *table = this->ui->SalesReportTable;
  QList<QList<QString>> purchases = this->database->getPurchases();
  for (int i = 0; i < purchases.count(); i++) {
    table->insertRow(i);
    for (int z = 0; z < purchasesColumnsToRead; z++) {
      QTableWidgetItem *tableItem = new QTableWidgetItem;
      tableItem->setText(purchases.at(i).at(z));
      table->setItem(i, z, tableItem);
    }
  }
  return;
}

void Dashboard::loadMembersTableFromDatabase() {
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

void Dashboard::loadInventoryTableFromDatabase() {
  const int inventoryColumnsToRead = 5;
  QTableWidget *table = this->ui->InventoryListTable;
//  QList<QList<QString>> inventory = this->database->getMembers();
//  for (int i = 0; i < inventory.count(); i++) {
//    table->insertRow(i);
//    for (int z = 0; z < inventoryColumnsToRead; z++) {
//      QTableWidgetItem *tableItem = new QTableWidgetItem;
//      tableItem->setText(inventory.at(i).at(z));
//      table->setItem(i, z, tableItem);
//    }
//  }
  return;
}


void Dashboard::on_button_importMembersFromFileSelection_clicked()
{
  this->database->importMembersFromFileSelection(this);
  loadMembersTableFromDatabase();
}

void Dashboard::on_button_importPurchasesFromFileSelection_clicked()
{
  this->database->importPurchasesFromFileSelection(this);
  loadPurchasesTableFromDatabase();
}
