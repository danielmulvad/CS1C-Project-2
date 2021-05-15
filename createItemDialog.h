#ifndef CREATEITEMDIALOG_H
#define CREATEITEMDIALOG_H

#include <QDialog>

namespace Ui {
  class CreateItemDialog;
}

class CreateItemDialog : public QDialog
{
  Q_OBJECT

public:
  explicit CreateItemDialog(QWidget *parent = nullptr);
  ~CreateItemDialog();

  QString getName() const;
  double getPrice() const;
private:
  Ui::CreateItemDialog *ui;
};

#endif // CREATEITEMDIALOG_H
