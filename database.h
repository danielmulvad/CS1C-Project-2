#ifndef DATABASE_H
#define DATABASE_H
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DbManager {
public:
  DbManager(const QString &path);

private:
  QSqlDatabase m_db;
};

#endif // DATABASE_H
