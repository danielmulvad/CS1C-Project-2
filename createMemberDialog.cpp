#include "createMemberDialog.h"
#include "ui_createMemberDialog.h"

CreateMemberDialog::CreateMemberDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreateMemberDialog) {
  ui->setupUi(this);
}

CreateMemberDialog::~CreateMemberDialog() { delete ui; }

QString CreateMemberDialog::getName() const {
  return this->ui->input_memberName->text();
}

int CreateMemberDialog::getId() const {
  return this->ui->input_id->text().toInt();
}

QString CreateMemberDialog::getType() const {
  return this->ui->input_type->currentText();
}

QDate CreateMemberDialog::getExpirationDate() const {
  return this->ui->input_expirationDate->date();
}
