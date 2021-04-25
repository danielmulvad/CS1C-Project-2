#include "database.h"

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
        "CREATE TABLE IF NOT EXISTS memberships (type STRING PRIMARY KEY UNIQUE NOT NULL, "
        "rebate DOUBLE NOT NULL, dues DOUBLE NOT NULL);");
    createMembershipsTable.exec(
          "INSERT INTO memberships (type, rebate, dues) VALUES ('Regular', 2.00, 65.00)"
    );
    createMembershipsTable.exec(
          "INSERT INTO memberships (type, rebate, dues) VALUES ('Executive', 0.00, 120.00)"
    );
    QSqlQuery createMembersTable;
    createMembersTable.exec(
        "CREATE TABLE IF NOT EXISTS members (name STRING NOT NULL, number INT "
        "PRIMARY KEY UNIQUE NOT NULL, type STRING NOT NULL REFERENCES "
        "memberships (type), expirationDate DATE NOT NULL)");
    QSqlQuery createPurchasesTable;
    createPurchasesTable.exec(
        "CREATE TABLE IF NOT EXISTS purchases (purchaseDate DATE NOT NULL, "
        "customerId INT REFERENCES members (number), productDescription "
        "STRING, productPrice DOUBLE, "
        "productQuantity INT)");
    QSqlDatabase::database().commit();
    this->listTables();
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

void DbManager::importPurchasesFromFileSelection(QWidget *widget) {
  const auto fileName = QFileDialog::getOpenFileName(widget, "Open Image", "",
                                                     "Image Files (*.xlsx)");
  qDebug() << fileName;
  return;
}
