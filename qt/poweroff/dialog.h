#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTime>
#include <QDebug>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);

    int countDownTime;

    ~Dialog();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
