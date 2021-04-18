#ifndef DATABASE_H
#define DATABASE_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>

class DbManager
{
public:
    DbManager(const QString& path);
private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
