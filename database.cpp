#include "database.h"

QString cleanInputText(QString string) {
  string.remove(QRegularExpression("(\\t)"));
  return string.trimmed();
}

DbManager::DbManager(const QString &path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);
  if (m_db.open()) {
    // success!
    qDebug() << "Database: connection ok";
    qDebug() << "Database path: " << path;
    QSqlDatabase::database().transaction();
    QSqlQuery createMembershipsTable;
    createMembershipsTable.exec(
        "CREATE TABLE IF NOT EXISTS memberships (type STRING PRIMARY KEY "
        "UNIQUE NOT NULL, "
        "rebate DOUBLE NOT NULL, dues DOUBLE NOT NULL);");
    createMembershipsTable.exec(
        "INSERT INTO memberships (type, rebate, dues) VALUES ('Regular', 0.00, "
        "65.00)");
    createMembershipsTable.exec(
        "INSERT INTO memberships (type, rebate, dues) VALUES ('Executive', "
        "2.00, 120.00)");
    QSqlQuery createMembersTable;
    createMembersTable.exec(
        "CREATE TABLE IF NOT EXISTS members (name STRING NOT NULL, number INT "
        "PRIMARY KEY UNIQUE NOT NULL, type STRING NOT NULL REFERENCES "
        "memberships (type) ON DELETE NO ACTION ON UPDATE NO ACTION, "
        "expirationDate DATE NOT NULL)");
    QSqlQuery createPurchasesTable;
    createPurchasesTable.exec(
        "CREATE TABLE IF NOT EXISTS purchases (purchaseDate DATE NOT NULL, "
        "customerId INT REFERENCES members (number) ON DELETE NO ACTION ON "
        "UPDATE NO ACTION, productDescription "
        "STRING, productPrice DOUBLE, "
        "productQuantity INT)");
    QSqlDatabase::database().commit();
  } else {
    qDebug() << "Could not open SQLite3 connection";
  }
};

DbManager::~DbManager() {
  if (m_db.isOpen())
    m_db.close();
};

void DbManager::listTables() {
  const QStringList tables = m_db.tables();
  auto it = tables.begin();
  while (it != tables.end()) {
    qDebug() << *it;
    ++it;
  }
}
void DbManager::importMembersFromFileSelection(QWidget *widget) {
  const auto fileName = QFileDialog::getOpenFileName(
      widget, "Open Purchases File", "", "Text Files (*.txt)");
  qDebug() << fileName;
  QFile inputFile(fileName);
  if (inputFile.open(QIODevice::ReadOnly)) {
    QTextStream in(&inputFile);
    QSqlDatabase::database().transaction();
    while (!in.atEnd()) {
      QString customerName, customerMembershipType;
      QDate customerMembershipExpiration;
      int customerMemberNumber;
      for (int column = 0; column < 4; column++) {
        QString line = cleanInputText(in.readLine());
        switch (column) {
        case 0:
          customerName = line;
          break;
        case 1:
          customerMemberNumber = line.toInt();
          break;
        case 2:
          customerMembershipType = line;
          break;
        case 3:
          customerMembershipExpiration = QDate::fromString(line, "MM/dd/yyyy");
          break;
        default:
          break;
        }
      }
      if (customerName != "" && customerMembershipExpiration.isValid())
        this->createMember(customerName, customerMemberNumber,
                           customerMembershipType,
                           customerMembershipExpiration);
    }
    inputFile.close();
    QSqlDatabase::database().commit();
  }
  return;
}

void DbManager::importPurchasesFromFileSelection(QWidget *widget) {
  const auto fileName = QFileDialog::getOpenFileName(
      widget, "Open Purchases File", "", "Text Files (*.txt)");
  qDebug() << fileName;
  QFile inputFile(fileName);
  if (inputFile.open(QIODevice::ReadOnly)) {
    QTextStream in(&inputFile);
    QSqlDatabase::database().transaction();
    while (!in.atEnd()) {
      QString date, itemPurchased;
      int customerId;
      double price, quantity;
      for (int column = 0; column < 5; column++) {
        QString line = cleanInputText(in.readLine());
        switch (column) {
        case 0:
          date = line;
          break;
        case 1:
          customerId = line.toInt();
          break;
        case 2:
          itemPurchased = line;
          break;
        case 3:
          price = line.toDouble();
          break;
        case 4:
          quantity = line.toDouble();
          break;
        default:
          break;
        }
      }
      if (date != "" && customerId != 0) {
        QSqlQuery createPurchase;
        createPurchase.prepare(
            "INSERT INTO purchases (purchaseDate, customerId, "
            "productDescription, "
            "productPrice, productQuantity) VALUES (:purchaseDate, "
            ":customerId, "
            ":productDescription, :productPrice, :productQuantity)");
        createPurchase.bindValue(":purchaseDate", date);
        createPurchase.bindValue(":customerId", customerId);
        createPurchase.bindValue(":productDescription", itemPurchased);
        createPurchase.bindValue(":productPrice", price);
        createPurchase.bindValue(":productQuantity", quantity);
        qDebug() << date << customerId << itemPurchased << price << quantity;
        createPurchase.exec();
      }
    }
    inputFile.close();
    QSqlDatabase::database().commit();
  }
  return;
}

QList<QList<QString>> DbManager::getMembers() {
  QList<QList<QString>> ret;
  QSqlQuery getMembersQuery;
  getMembersQuery.exec("SELECT * FROM members");
  while (getMembersQuery.next()) {
    QList<QString> temp;
    temp.push_back(getMembersQuery.value(0).toString());
    temp.push_back(getMembersQuery.value(1).toString());
    temp.push_back(getMembersQuery.value(2).toString());
    temp.push_back(getMembersQuery.value(3).toString());
    ret.push_back(temp);
  }
  return ret;
}

QStringList DbManager::getMemberNames() const {
  QStringList ret;
  QSqlQuery getMemberNamesQuery;
  if (getMemberNamesQuery.exec("SELECT DISTINCT name FROM members")) {
    while (getMemberNamesQuery.next()) {
      ret.push_back(getMemberNamesQuery.value(0).toString());
    }
  }
  return ret;
}

bool DbManager::createMember(const QString &customerName,
                             const int &customerMemberNumber,
                             const QString &customerMembershipType,
                             const QDate &customerMembershipExpiration) {
  QSqlQuery createMember;
  createMember.prepare(
      "INSERT INTO members (name, number, type, expirationDate) VALUES "
      "(:name, :number, :type, :expirationDate)");
  createMember.bindValue(":name", customerName);
  createMember.bindValue(":number", customerMemberNumber);
  createMember.bindValue(":type", customerMembershipType);
  createMember.bindValue(":expirationDate",
                         customerMembershipExpiration.toString("MM/dd/yyyy"));
  qDebug() << customerName << customerMemberNumber << customerMembershipType
           << customerMembershipExpiration;
  return createMember.exec();
}

bool DbManager::deleteMember(const int &id) {
  QSqlQuery deleteMember;
  deleteMember.prepare("DELETE FROM members WHERE number=:number");
  deleteMember.bindValue(":number", id);
  return deleteMember.exec();
}

QStringList DbManager::getProductDescriptions() const {
  QStringList ret;
  QSqlQuery getProductDescriptionsQuery;
  if (getProductDescriptionsQuery.exec(
          "SELECT DISTINCT productDescription FROM purchases")) {
    while (getProductDescriptionsQuery.next()) {
      ret.push_back(getProductDescriptionsQuery.value(0).toString());
    }
  }
  return ret;
}

QList<QList<QString>> DbManager::getPurchases() {
  QList<QList<QString>> ret;
  QSqlQuery getPurchasesQuery;

  if (getPurchasesQuery.exec("SELECT * FROM purchases")) {
    while (getPurchasesQuery.next()) {
      QList<QString> temp;
      temp.push_back(getPurchasesQuery.value(0).toString());
      temp.push_back(getPurchasesQuery.value(1).toString());
      temp.push_back(getPurchasesQuery.value(2).toString());
      temp.push_back(getPurchasesQuery.value(3).toString());
      temp.push_back(getPurchasesQuery.value(4).toString());
      ret.push_back(temp);
    }
  }
  return ret;
}

bool DbManager::createPurchase(const QDate &date, const int &customerId,
                               const QString &name, const double &productPrice,
                               const int &productQuantity) {
  QSqlQuery createPurchase;
  createPurchase.prepare(
      "INSERT INTO purchases (purchaseDate, customerId, productDescription, "
      "productPrice, productQuantity) VALUES "
      "(:purchaseDate, :customerId, :productDescription, :productPrice, "
      ":productQuantity)");
  createPurchase.bindValue(":purchaseDate", date.toString("MM/dd/yyyy"));
  createPurchase.bindValue(":customerId", customerId);
  createPurchase.bindValue(":productDescription", name);
  createPurchase.bindValue(":productPrice", productPrice);
  createPurchase.bindValue(":productQuantity", productQuantity);
  return createPurchase.exec();
}

bool DbManager::deletePurchase(const QString &productDescription) {
  QSqlQuery deletePurchase;
  deletePurchase.prepare(
      "DELETE FROM purchases WHERE productDescription=:productDescription");
  deletePurchase.bindValue(":productDescription", productDescription);
  return deletePurchase.exec();
}
