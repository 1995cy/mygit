#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    ui->timeEdit->setTime(QTime::currentTime());
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_okButton_clicked()
{
    QTime time = ui->timeEdit->time();
    QTime now_time = QTime::currentTime();

    int hour = time.hour();
    int minute = time.minute();

    int hour_now = now_time.hour();
    int minute_now = now_time.minute();

    int setTimer = hour * 3600 + minute * 60;
    int now = hour_now *3600 + minute_now * 60;

    setTimer -= now;

    if(setTimer >= 0){
        this->countDownTime = setTimer;
    }else {
        this->countDownTime = 24 * 3600 + setTimer;
    }
    this->close();
}

void Dialog::on_cancelButton_clicked()
{
    this->close();
}
