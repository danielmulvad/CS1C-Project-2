#ifndef COSTCOPAGE_H
#define COSTCOPAGE_H

#include <QDialog>
#include <database.h>

namespace Ui {
class CostcoPage;
}

class CostcoPage : public QDialog {
  Q_OBJECT

public:
  CostcoPage(DbManager *db, QWidget *parent = nullptr);
  ~CostcoPage();

private slots:
  void on_button_importPurchasesFromFileSelection_clicked();

  void on_button_importMembersFromFileSelection_clicked();

private:
  Ui::CostcoPage *ui;
  DbManager *database;

  void loadPurchasesTableFromDatabase();
  void loadMembersTableFromDatabase();
  void loadInventoryTableFromDatabase();
};

#endif // COSTCOPAGE_H
