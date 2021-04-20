#ifndef DATABASE_H
#define DATABASE_H
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DbManager {
 public:
  DbManager(const QString&);
  ~DbManager();
  void listTables();

 private:
  QSqlDatabase m_db;
};

#endif  // DATABASE_H
