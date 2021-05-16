#include "dashboard.h"

#include "ui_dashboard.h"
#include <QDate>
#include <iostream>

Dashboard::Dashboard(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Dashboard) {
  ui->setupUi(this);
}

Dashboard::Dashboard(DbManager *db, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Dashboard) {
  this->database = db;
  ui->setupUi(this);
  this->loadMembersTableFromDatabase();
  this->salesReportByDay();
  this->loadInventoryTableFromDatabase();
  this->membershipExpirationByMonth();
  this->loadMemberConversions();
}

Dashboard::~Dashboard() {
  delete ui;
  delete database;
  delete createMemberDialog;
  delete createItemDialog;
}

void Dashboard::loadPurchasesTableFromDatabase() {
  const int purchasesColumnsToRead = 5;
  QTableWidget *table = this->ui->SalesReportTable;
  table->setRowCount(0);
  QList<QList<QString>> purchases = this->database->getPurchases();
  for (int i = 0; i < purchases.count(); i++) {
    table->insertRow(i);
    for (int z = 0; z < purchasesColumnsToRead; z++) {
      QTableWidgetItem *tableItem = new QTableWidgetItem;
      if (z == 1)
        tableItem->setText(memberNameFromIDNum(purchases.at(i).at(z)));
      else
        tableItem->setText(purchases.at(i).at(z));
      table->setItem(i, z, tableItem);
    }

    QTableWidgetItem *type = new QTableWidgetItem;

    if (isRegularMember(purchases.at(i).at(1))) {
      type->setText("Regular");
    } else {
      type->setText("Executive");
    }

    table->setItem(i, 5, type);
  }
  return;
}

void Dashboard::loadMembersTableFromDatabase() {
  const int membersColumnsToRead = 4;
  QTableWidget *table = this->ui->MembershipInformationTable;
  table->setRowCount(0);
  QList<QList<QString>> members = this->database->getMembers();
  for (int i = 0; i < members.count(); i++) {
    table->insertRow(i);
    for (int z = 0; z < membersColumnsToRead; z++) {
      QTableWidgetItem *tableItem = new QTableWidgetItem;
      tableItem->setText(members.at(i).at(z));
      table->setItem(i, z, tableItem);
    }
  }
  loadMemberPurchaseLog();
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
  table->setRowCount(0);
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
        total +=
            inventory.at(j).at(3).toDouble() * inventory.at(j).at(4).toInt();
        grandTotal += inventory.at(j).at(3).toDouble() *
                      inventory.at(j).at(4).toInt() * 1.0775;
      }
    }

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText(QString::number(total * 1.0775, 'f', 2));
    table->setItem(i, 4, item);

    if (table->item(i, 2)->text() == "Executive") {
      QTableWidgetItem *rebate = new QTableWidgetItem;
      rebate->setText(QString::number(total * 0.02, 'f', 2));
      table->setItem(i, 5, rebate);

      qDebug() << total << " " << rebate->text();
    }
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

void Dashboard::loadMemberConversions() {
  this->ui->regularTable->setRowCount(0);
  this->ui->executiveTable->setRowCount(0);

  this->ui->regularTable->setRowCount(0);
  this->ui->executiveTable->setRowCount(0);

  // set column width
  this->ui->regularTable->setColumnWidth(0, 256);
  this->ui->executiveTable->setColumnWidth(0, 256);

  QTableWidget *table = this->ui->MembershipInformationTable;
  QTableWidget *exec = this->ui->executiveTable;
  QTableWidget *reg = this->ui->regularTable;
  int limit = 120 - 65;

  for (int i = 0; i < table->rowCount(); i++) {
    if (table->item(i, 2)->text() == "Executive") {
      if (table->item(i, 5)->text().toDouble() < limit) {
        exec->insertRow(exec->rowCount());
        QTableWidgetItem *member = new QTableWidgetItem;
        member->setText(table->item(i, 0)->text());
        exec->setItem(exec->rowCount() - 1, 0, member);
      }
    } else {
      if (table->item(i, 4)->text().toDouble() / 1.0775 * 0.02 > limit) {
        reg->insertRow(reg->rowCount());
        QTableWidgetItem *member = new QTableWidgetItem;
        member->setText(table->item(i, 0)->text());
        reg->setItem(reg->rowCount() - 1, 0, member);
      }
    }
  }

  qDebug() << "out of for loop";

  this->ui->regular->setText("Total Regular Conversions: " +
                             QString::number(reg->rowCount()));
  this->ui->executive->setText("Total Executive Conversions: " +
                               QString::number(exec->rowCount()));
}

void Dashboard::on_button_importMembersFromFileSelection_clicked() {
  this->database->importMembersFromFileSelection(this);
  loadMembersTableFromDatabase();
}

void Dashboard::on_button_importPurchasesFromFileSelection_clicked() {
  this->database->importPurchasesFromFileSelection(this);
  salesReportByDay();
  loadInventoryTableFromDatabase();
  loadMemberPurchaseLog();
  // loadMemberConversions();
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
  }
}

void Dashboard::on_button_deleteMember_clicked() {
  QTableWidget *table = this->ui->MembershipInformationTable;
  const int currentRowIndex = table->currentRow();
  qDebug() << "Row index " << currentRowIndex;
  if (currentRowIndex >= 0) {
    const int memberId = table->item(currentRowIndex, 1)->text().toInt();
    if (database->deleteMember(memberId)) {
      table->removeRow(currentRowIndex);
    }
  }
}

void Dashboard::salesReportByDay() {
  int daytoReport = this->ui->days->currentIndex() + 1;
  QList<QList<QString>> purchases = this->database->getPurchases();

  QString datetoReport = "4/" + QString::number(daytoReport) + "/2021";
  QTableWidget *table = this->ui->SalesReportTable;
  // QList<QList<QString>> purchases = this->database->getPurchases(0);
  double total = 0.0;
  QVector<QString> regularShoppers;
  QVector<QString> executiveShoppers;
  int numOfReg = 0;
  int numOfExec = 0;

  // disable sorting
  this->ui->InventoryListTable->setSortingEnabled(false);

  table->setRowCount(0);
  table->setColumnWidth(0, 90);
  table->setColumnWidth(3, 90);
  table->setColumnWidth(4, 140);
  table->setColumnWidth(5, 90);

  if (daytoReport == 8) {
    loadPurchasesTableFromDatabase();
    this->ui->dailyRevenue->setText("");
    this->ui->numReg->setText("");
    this->ui->numExec->setText("");
    return;
  }

  for (int i = 0; i < purchases.count(); i++) {

    if (purchases.at(i).at(0) == datetoReport) {

      table->insertRow(table->rowCount());
      QTableWidgetItem *date = new QTableWidgetItem;
      QTableWidgetItem *name = new QTableWidgetItem;
      QTableWidgetItem *item = new QTableWidgetItem;
      QTableWidgetItem *price = new QTableWidgetItem;
      QTableWidgetItem *quantity = new QTableWidgetItem;
      QTableWidgetItem *type = new QTableWidgetItem;

      date->setText(purchases.at(i).at(0));
      name->setText(memberNameFromIDNum(purchases.at(i).at(1)));
      item->setText(purchases.at(i).at(2));
      price->setText(purchases.at(i).at(3));
      quantity->setText(purchases.at(i).at(4));
      type->setText(purchases.at(i).at(5));

      table->setItem(table->rowCount() - 1, 0, date);
      table->setItem(table->rowCount() - 1, 1, name);
      table->setItem(table->rowCount() - 1, 2, item);
      table->setItem(table->rowCount() - 1, 3, price);
      table->setItem(table->rowCount() - 1, 4, quantity);
      table->setItem(table->rowCount() - 1, 5, type);

      total += price->text().toDouble() * quantity->text().toInt();

      bool found = false;

      if (isRegularMember(purchases.at(i).at(1))) {
        for (int j = 0; j < regularShoppers.count(); j++) {
          if (purchases.at(i).at(1) == regularShoppers.at(j)) {
            found = true;
          }
        }
      } else {
        for (int j = 0; j < executiveShoppers.count(); j++) {
          if (purchases.at(i).at(1) == executiveShoppers.at(j))
            found = true;
        }
      }

      if (!found && isRegularMember(purchases.at(i).at(1))) {
        regularShoppers.push_back(purchases.at(i).at(1));
        numOfReg++;
      } else if (!found && !isRegularMember(purchases.at(i).at(1))) {
        executiveShoppers.push_back(purchases.at(i).at(1));
        numOfExec++;
      }
    }
  }

  this->ui->dailyRevenue->setText("Total Daily Revenue: $" +
                                  QString::number(total * 1.0775, 'f', 2));
  this->ui->numReg->setText("Number of Unique Regular Shoppers: " +
                            QString::number(numOfReg));
  this->ui->numExec->setText("Number of Unique Executive Shoppers: " +
                             QString::number(numOfExec));

  // enable sorting
  this->ui->InventoryListTable->setSortingEnabled(true);
}

bool Dashboard::isRegularMember(QString IDNum) {
  QList<QList<QString>> members = this->database->getMembers();
  for (int i = 0; i < members.count(); i++) {
    if (members.at(i).at(1) == IDNum) {
      if (members.at(i).at(2) == "Regular") {
        return true;
      } else {
        return false;
      }
    }
  }
  qDebug() << "can't find member";
  return true;
}

QString Dashboard::memberNameFromIDNum(QString IDNum) {
  QList<QList<QString>> members = this->database->getMembers();
  for (int i = 0; i < members.count(); i++) {
    if (members.at(i).at(1) == IDNum) {
      return members.at(i).at(0);
    }
  }
  qDebug() << "can't find member";
  return "";
}

void Dashboard::on_button_createItem_clicked() {
  this->createItemDialog = new CreateItemDialog();
  this->createItemDialog->show();
  if (this->createItemDialog->exec() == QDialog::Accepted) {
    QString name = this->createItemDialog->getName();
    double price = this->createItemDialog->getPrice();
    if (database->createPurchase(QDate::currentDate(), 0, name, price, 0)) {
      this->loadInventoryTableFromDatabase();
    }
  }
}

void Dashboard::on_button_deleteItem_clicked() {
  QTableWidget *table = this->ui->InventoryListTable;
  const int currentRowIndex = table->currentRow();
  const QString productDescription = table->item(currentRowIndex, 0)->text();
  if (currentRowIndex >= 0) {
    if (database->deletePurchase(productDescription)) {
      table->removeRow(currentRowIndex);
      this->loadInventoryTableFromDatabase();
    }
  }
}
