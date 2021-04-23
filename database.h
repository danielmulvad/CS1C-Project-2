#ifndef DATABASE_H
#define DATABASE_H
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QFileDialog>

class DbManager {
public:
  DbManager(const QString &);
  ~DbManager();
  void listTables();
  void importPurchasesFromFileSelection(QWidget * widget);

private:
  QSqlDatabase m_db;
};

#endif // DATABASE_H
