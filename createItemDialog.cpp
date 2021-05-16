#include "createItemDialog.h"
#include "ui_createItemDialog.h"

CreateItemDialog::CreateItemDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreateItemDialog) {
  ui->setupUi(this);
}

CreateItemDialog::~CreateItemDialog() { delete ui; }

QString CreateItemDialog::getName() const {
  return this->ui->input_itemName->text();
}

double CreateItemDialog::getPrice() const {
  return this->ui->input_itemPrice->text().toDouble();
}
