#ifndef COSTCOPAGE_H
#define COSTCOPAGE_H

#include <QDialog>

namespace Ui {
class CostcoPage;
}

class CostcoPage : public QDialog {
  Q_OBJECT

public:
  explicit CostcoPage(QWidget *parent = nullptr);
  ~CostcoPage();

private:
  Ui::CostcoPage *ui;
};

#endif // COSTCOPAGE_H
