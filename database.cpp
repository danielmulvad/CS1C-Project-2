#include "database.h"

DbManager::DbManager(const QString &path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);
  if (m_db.open()) {
    // success!
    qDebug() << "Database: connection ok";
    qDebug() << "Database path: " << path;
    QSqlQuery query;
    query.prepare(
        "CREATE TABLE IF NOT EXISTS purchases (purchaseDate DATE NOT NULL, "
        "customerId INT, productDescription STRING, productPrice DOUBLE, "
        "productQuantity INT)");
    query.exec();
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
