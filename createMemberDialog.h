#ifndef CREATEMEMBERDIALOG_H
#define CREATEMEMBERDIALOG_H

#include <QDialog>

namespace Ui {
class CreateMemberDialog;
}

class CreateMemberDialog : public QDialog {
  Q_OBJECT

public:
  explicit CreateMemberDialog(QWidget *parent = nullptr);
  ~CreateMemberDialog();

  QString getName() const;
  int getId() const;
  QString getType() const;
  QDate getExpirationDate() const;

private:
  Ui::CreateMemberDialog *ui;
};

#endif // CREATEMEMBERDIALOG_H
