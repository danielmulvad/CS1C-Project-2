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
  this->loadMemberPurchaseLog();
  this->membershipExpirationByMonth();
}

Dashboard::~Dashboard() {
  delete ui;
  delete database;
  delete createMemberDialog;
}

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
    if (index == -1 && inventory.at(i).at(0) != "") {
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
    else if (inventory.at(i).at(0) != "") {
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

void Dashboard::loadMemberPurchaseLog() {

  QList<QList<QString>> inventory = this->database->getPurchases();
  QTableWidget *table = this->ui->MembershipInformationTable;
  double grandTotal = 0.0;

  for (int i = 0; i < table->rowCount(); i++) {

    QString memberNum = table->item(i, 1)->text();
    qDebug() << memberNum;
    double total = 0.0;

    for (int j = 0; j < inventory.count(); j++) {
      if (inventory.at(j).at(1) == memberNum) {
        total += inventory.at(j).at(3).toDouble() *
                 inventory.at(j).at(4).toInt() * 1.775;
        grandTotal += inventory.at(j).at(3).toDouble() *
                      inventory.at(j).at(4).toInt() * 1.775;
      }
    }

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(QString::number(total, 'f', 2));
    table->setItem(i, 4, item);
  }

  this->ui->GrandTotal->setText("Grand Total: $" +
                                QString::number(grandTotal, 'f', 2));
}

void Dashboard::searchForItem() {

  this->ui->name->setText("");
  this->ui->quantity->setText("");
  this->ui->revenue->setText("");
  this->ui->errorMessage->setText("");

  QString searchItem = this->ui->item->text();
  qDebug() << "Search Item: " + searchItem;
  QTableWidget *table = this->ui->InventoryListTable;

  for (int i = 0; i < table->rowCount(); i++) {
    if (searchItem == table->item(i, 0)->text()) {
      qDebug() << "found item";
      this->ui->name->setText(searchItem);
      this->ui->quantity->setText("Quantity Sold: " +
                                  table->item(i, 2)->text());
      this->ui->revenue->setText("Total Revenue: $" +
                                 table->item(i, 3)->text());
      return;
    }
  }

  this->ui->errorMessage->setText("Could not find item.");
}

void Dashboard::searchForMember() {

  this->ui->memberName->setText("");
  this->ui->purchases->setText("");
  this->ui->error->setText("");

  QString searchMember = this->ui->member->text();
  qDebug() << "Search Member: " << searchMember;
  QTableWidget *table = this->ui->MembershipInformationTable;

  for (int i = 0; i < table->rowCount(); i++) {
    if (searchMember == table->item(i, 0)->text() ||
        searchMember == table->item(i, 1)->text()) {
      qDebug() << "found item";
      this->ui->memberName->setText(table->item(i, 0)->text());
      this->ui->purchases->setText("Total Purchases: $" +
                                   table->item(i, 4)->text());
      return;
    }
  }

  this->ui->error->setText("Could not find member.");
}

void Dashboard::membershipExpirationByMonth() {

  this->ui->expirationTable->setRowCount(0);

  // set column width
  for (int i = 0; i < 2; i++) {
    this->ui->expirationTable->setColumnWidth(i, 350);
  }

  int expMonth = this->ui->months->currentIndex() + 1;
  QList<QList<QString>> members = this->database->getMembers();
  QTableWidget *table = this->ui->expirationTable;

  for (int i = 0; i < members.count(); i++) {
    QString name = members.at(i).at(0);
    QDate expDate = QDate::fromString(members.at(i).at(3), "MM/dd/yyyy");
    int memExpMon = expDate.month();

    if (expMonth == memExpMon) {

      table->insertRow(table->rowCount());

      QTableWidgetItem *memName = new QTableWidgetItem;
      QTableWidgetItem *costToRenew = new QTableWidgetItem;

      memName->setText(name);

      if (members.at(i).at(2) == "Executive")
        costToRenew->setText("$120.00");
      else
        costToRenew->setText("$65.00");

      table->setItem(table->rowCount() - 1, 0, memName);
      table->setItem(table->rowCount() - 1, 1, costToRenew);
    }
  }
}

void Dashboard::on_button_importMembersFromFileSelection_clicked() {
  this->database->importMembersFromFileSelection(this);
  loadMembersTableFromDatabase();
}

void Dashboard::on_button_importPurchasesFromFileSelection_clicked() {
  this->database->importPurchasesFromFileSelection(this);
  loadPurchasesTableFromDatabase();
  loadInventoryTableFromDatabase();
  loadMemberPurchaseLog();
}

void Dashboard::on_button_createMember_clicked() {
  this->createMemberDialog = new CreateMemberDialog();
  this->createMemberDialog->show();
  if (this->createMemberDialog->exec() == QDialog::Accepted) {
    QString name = this->createMemberDialog->getName();
    QString type = this->createMemberDialog->getType();
    int id = this->createMemberDialog->getId();
    QDate expDate = this->createMemberDialog->getExpirationDate();
    this->database->createMember(name, id, type, expDate);
    this->ui->MembershipInformationTable->setRowCount(0);
    loadMembersTableFromDatabase();
    loadMemberPurchaseLog();
    qDebug() << "Accepted dialog " << name << type << id << expDate;
  }
}
