#include "createMemberDialog.h"
#include "ui_createMemberDialog.h"

CreateMemberDialog::CreateMemberDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreateMemberDialog) {
  ui->setupUi(this);
}

CreateMemberDialog::~CreateMemberDialog() { delete ui; }
