#ifndef ADDMEMBER_H
#define ADDMEMBER_H

#include "costcopage.h"
#include <QDialog>
#include <vector>
namespace Ui {
class AddMember;
}

class AddMember : public QDialog {
  Q_OBJECT
public:
  explicit AddMember(QWidget *parent = nullptr);
  ~AddMember();

private slots:
  void on_pushButton_clicked();
  void on_addFromFileButton_clicked();

private:
  Ui::AddStadium *ui;
};

#endif // ADDMEMBER_H
