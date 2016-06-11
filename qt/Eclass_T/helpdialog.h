#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMovie>

namespace Ui {
class helpDialog;
}

class helpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit helpDialog(QWidget *parent = 0);
    ~helpDialog();

private:
    Ui::helpDialog *ui;
};

#endif // HELPDIALOG_H
