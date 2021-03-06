#ifndef DATABASE_H
#define DATABASE_H
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QRegularExpression>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DbManager {
public:
  DbManager(const QString &);
  ~DbManager();
  void listTables();
  QList<QList<QString>> getMembers();
  QStringList getMemberNames() const;
  QStringList getProductDescriptions() const;
  QList<QList<QString>> getPurchases();
  bool createPurchase(const QDate &date, const int &customerId,
                      const QString &name, const double &productPrice,
                      const int &productQuantity);
  bool deletePurchase(const QString &productDescription);
  void importMembersFromFileSelection(QWidget *widget);
  void importPurchasesFromFileSelection(QWidget *widget);
  bool createMember(const QString &name, const int &id, const QString &type,
                    const QDate &expDate);
  bool deleteMember(const int &id);

private:
  QSqlDatabase m_db;
};

#endif // DATABASE_H
