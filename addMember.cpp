#include "addMember.h"
#include "database.h"
#include <QSpinBox>

AddMember::AddMember(QWidget *parent) : QDialog(parent), ui(new Ui::AddMember) {
  ui->setupUi(this);
}

AddMember::~AddMember() { delete ui; }

void AddMember::on_addFromFileButton_clicked() {}
