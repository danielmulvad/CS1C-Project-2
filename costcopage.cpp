#include "costcopage.h"
#include "ui_costcopage.h"

CostcoPage::CostcoPage(DbManager *db, QWidget *parent)
     : QDialog(parent), ui(new Ui::CostcoPage) {
  this->database = db;
  ui->setupUi(this);
}

CostcoPage::~CostcoPage() { delete ui; }

void CostcoPage::on_button_importPurchasesFromFileSelection_clicked()
{
  // This is an example database function. -- only committed to update progress & show implementation
  this->database->listTables();
}
