#ifndef CREATEPURCHASEDIALOG_H
#define CREATEPURCHASEDIALOG_H

#include <QDialog>
#include "database.h"

namespace Ui {
  class CreatePurchaseDialog;
}

class CreatePurchaseDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CreatePurchaseDialog(DbManager *db, QWidget *parent = nullptr);
  ~CreatePurchaseDialog();

  QString getName() const;
  QString getProductDescription() const;
  double getPrice() const;
  int getQuantity() const;

private:
  Ui::CreatePurchaseDialog *ui;
  DbManager *database;
};

#endif // CREATEPURCHASEDIALOG_H
