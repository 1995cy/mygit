#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

#include <QDialog>
#include <QTime>
#include <QDebug>

namespace Ui {
class timeDialog;
}

class timeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit timeDialog(QWidget *parent = 0);
    int downtime;
    ~timeDialog();

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::timeDialog *ui;
};

#endif // TIMEDIALOG_H
