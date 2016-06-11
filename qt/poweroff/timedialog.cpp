#include "timedialog.h"
#include "ui_timedialog.h"

timeDialog::timeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::timeDialog)
{
    ui->setupUi(this);
}

timeDialog::~timeDialog()
{
    delete ui;
}


void timeDialog::on_okButton_clicked()
{
    QTime time = ui->timeEdit->time();
    int hour = time.hour();
    int minute = time.minute();

    this->downtime = hour * 3600 + minute * 60;

    this->close();
}

void timeDialog::on_cancelButton_clicked()
{
    this->close();
}
