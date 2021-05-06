#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "database.h"
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

private:
  Ui::Dashboard *ui;
  DbManager *database;

  void loadPurchasesTableFromDatabase();
  void loadMembersTableFromDatabase();
  void loadInventoryTableFromDatabase();

  void prepInventoryTable();
  QList<QList<QString>> getInventory();
};
#endif // DASHBOARD_H