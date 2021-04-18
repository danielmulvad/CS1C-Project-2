#include "costcopage.h"
#include "ui_costcopage.h"

CostcoPage::CostcoPage(QWidget *parent)
    : QDialog(parent), ui(new Ui::CostcoPage) {
  ui->setupUi(this);
}

CostcoPage::~CostcoPage() { delete ui; }
