#ifndef DATABASE_H
#define DATABASE_H
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QRegularExpression>

class DbManager {
public:
  DbManager(const QString &);
  ~DbManager();
  void listTables();
  void importMembersFromFileSelection(QWidget *widget);
  void importPurchasesFromFileSelection(QWidget *widget);

private:
  QSqlDatabase m_db;
};

#endif // DATABASE_H
