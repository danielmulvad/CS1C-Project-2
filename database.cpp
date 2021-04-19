#include "database.h"

DbManager::DbManager(const QString &path) {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);
  if (!m_db.open()) {
    qDebug() << "Error: connection with database failed";
    return;
  }
  qDebug() << "Database: connection ok";
  QSqlQuery query;
  auto ok =
      query.exec("CREATE TABLE test(id INTEGER PRIMARY KEY) IF NOT EXISTS");
  if (ok) {
    qDebug() << "Exec worked";
  } else {
    qDebug() << "Exec did not work";
  }
  ok = query.exec("INSERT INTO test (id) VALUES (1000)");
  if (ok) {
    qDebug() << "Exec worked";
  } else {
    qDebug() << "Exec did not work";
  }
};
