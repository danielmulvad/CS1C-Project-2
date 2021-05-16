#include "createPurchaseDialog.h"
#include "ui_createPurchaseDialog.h"

CreatePurchaseDialog::CreatePurchaseDialog(DbManager *db, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CreatePurchaseDialog)
{
  database = db;
  ui->setupUi(this);
  const QStringList products = database->getProductDescriptions();
  const QStringList members = database->getMemberNames();
  QComboBox* availableProductsBox = ui->combobox_availableProducts;
  QComboBox* availableMembers = ui->combobox_availableMembers;
  availableProductsBox->addItems(products);
  availableMembers->addItems(members);
}

CreatePurchaseDialog::~CreatePurchaseDialog()
{
  delete ui;
}

QString CreatePurchaseDialog::getName() const {
  return ui->combobox_availableMembers->currentText();
}
QString CreatePurchaseDialog::getProductDescription() const {
  return ui->combobox_availableProducts->currentText();
}
double CreatePurchaseDialog::getPrice() const {
  return ui->input_purchasePrice->text().toDouble();
}
int CreatePurchaseDialog::getQuantity() const {
  return ui->input_purchaseQuantity->text().toInt();
}
