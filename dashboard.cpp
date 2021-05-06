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

void Dashboard::prepInventoryTable() {

  // set column width
  for (int i = 0; i < 4; i++) {
    this->ui->InventoryListTable->setColumnWidth(i, 180);
  }

  // disable sorting
  this->ui->InventoryListTable->setSortingEnabled(false);

  // clear current table
  this->ui->InventoryListTable->clearSelection();
  this->ui->InventoryListTable->disconnect();
  this->ui->InventoryListTable->clearContents();
  this->ui->InventoryListTable->setRowCount(0);
}

QList<QList<QString>> Dashboard::getInventory() {

  QList<QList<QString>> inventory = this->database->getPurchases();
  QList<QList<QString>> combined;

  for (int i = 0; i < inventory.count(); i++) {

    int index = -1;

    // Search if the item was previously purchased
    for (int j = 0; j < combined.count(); j++) {
      if (inventory.at(i).at(2) == combined.at(j).at(0)) {
        index = j;
      }
    }

    // add new item
    if (index == -1 && inventory.at(i).at(0) != NULL) {
      QList<QString> temp;
      temp.push_back(inventory.at(i).at(2));
      temp.push_back(QString::number(inventory.at(i).at(3).toDouble(), 'f', 2));
      temp.push_back(inventory.at(i).at(4));
      temp.push_back(QString::number(
          (inventory.at(i).at(3).toDouble() * inventory.at(i).at(4).toInt()),
          'f', 2));
      qDebug() << inventory.at(i).at(3) << inventory.at(i).at(4);
      combined.push_back(temp);
    }

    // update current item if not a NULL item
    else if (inventory.at(i).at(0) != NULL) {
      combined[index][2] = QString::number(combined.at(index).at(2).toInt() +
                                           inventory.at(i).at(4).toInt());
      combined[index][3] = QString::number(
          (combined.at(index).at(2).toInt() * inventory.at(i).at(3).toDouble()),
          'f', 2);
    }
  }

  return combined;
}

void Dashboard::loadInventoryTableFromDatabase() {

  QList<QList<QString>> combined = getInventory();
  prepInventoryTable();

  QTableWidget *table = this->ui->InventoryListTable;
  for (int i = 0; i < combined.count(); i++) {
    table->insertRow(table->rowCount());
    for (int k = 0; k < 4; k++) {
      QTableWidgetItem *tableItem = new QTableWidgetItem;
      tableItem->setText(combined.at(table->rowCount() - 1).at(k));
      table->setItem(i, k, tableItem);
    }
  }

  // enable sorting again
  this->ui->InventoryListTable->setSortingEnabled(true);
  table->sortItems(0, Qt::AscendingOrder);
}

void Dashboard::on_button_importMembersFromFileSelection_clicked() {
  this->database->importMembersFromFileSelection(this);
  loadMembersTableFromDatabase();
}

void Dashboard::on_button_importPurchasesFromFileSelection_clicked() {
  this->database->importPurchasesFromFileSelection(this);
  loadPurchasesTableFromDatabase();
  loadInventoryTableFromDatabase();
}
