#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "createMemberDialog.h"
#include "database.h"
#include <QDate>
#include <QMainWindow>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dashboard;
}
QT_END_NAMESPACE

class Dashboard : public QMainWindow {
  Q_OBJECT

public:
  Dashboard(QWidget *parent = nullptr);
  Dashboard(DbManager *, QWidget *parent = nullptr);
  ~Dashboard();

private slots:
  void on_button_importMembersFromFileSelection_clicked();
  void on_button_importPurchasesFromFileSelection_clicked();

  void on_button_createMember_clicked();

  void searchForItem();
  void searchForMember();

  void membershipExpirationByMonth();

  void on_button_deleteMember_clicked();

private:
  Ui::Dashboard *ui;
  CreateMemberDialog *createMemberDialog;
  DbManager *database;

  void loadPurchasesTableFromDatabase();
  void loadMembersTableFromDatabase();
  void loadInventoryTableFromDatabase();

  void prepInventoryTable();
  QList<QList<QString>> getInventory();

  void loadMemberPurchaseLog();
};
#endif // DASHBOARD_H
