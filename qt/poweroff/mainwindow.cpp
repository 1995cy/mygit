#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "timedialog.h"
#include <QTimer>
#include <QString>

int downtime = 0;
QTimer *timer;
bool done = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->cancelButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//到设置的时间关机
void MainWindow::on_dateButton_clicked()
{
    Dialog *d = new Dialog;
    d->exec();
    int t =  d->countDownTime;

    if(t <= 0){
        return;
    }

    downtime = t;

    countdownDO();

    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(showTimeDown()));
    while(t > 0){
        timer->start(1000);
        t--;
    }
}

//当设置的一段时间过去以后进行关机
void MainWindow::on_timeButton_clicked()
{
    timeDialog *ti = new timeDialog;
    ti->exec();

    int t = ti->downtime;
    if(t <= 0){
        return;
    }

    downtime = t;

    countdownDO();

    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(showTimeDown()));
    while(t > 0){
        timer->start(1000);
        t--;
    }
}

//取消关机
void MainWindow::on_cancelButton_clicked()
{
    if(done == true){
         system("shutdown -a");
         done = false;
    }

    ui->cancelButton->setEnabled(false);
    ui->dateButton->setEnabled(true);
    ui->timeButton->setEnabled(true);
    timer->stop();
    downtime = 0;
    ui->label->setText("关机计划已经停止");
}

//显示倒计时
void MainWindow::showTimeDown(){
    //qDebug()<<done;
    if(downtime <= 60 && done == false){
        QString st = "shutdown -s -t "+QString::number(downtime);
        QByteArray ba = st.toLatin1();
        char *string = ba.data();
        system(string);
        done = true;
    }
    if(downtime > 0){
        QString str = "系统将于"+QString::number(downtime);
              str += "秒后关机";
        downtime--;
        ui->label->setText(str);
    }else{
        ui->label->setText("系统正在关机......");
    }

}

//倒计时的准备工作
void MainWindow::countdownDO(){
    timer = new QTimer(this);

    ui->dateButton->setEnabled(false);
    ui->timeButton->setEnabled(false);
    ui->cancelButton->setEnabled(true);

    QString str = "系统将于"+QString::number(downtime);
          str += "秒后关机";
    downtime--;
    ui->label->setText(str);
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(downtime == 0){
        event->accept();
    }else if(done == true){
        QMessageBox msgBox;
        msgBox.setText("关机计划已经启动");
        msgBox.setInformativeText("是否关闭关机计划");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Ok:
              on_cancelButton_clicked();
              event->accept();
              break;
          case QMessageBox::Discard:
              event->accept();
              break;
          case QMessageBox::Cancel:
          default:
              event->ignore();
              break;
        }
    }else {
        QMessageBox msgBox;
        msgBox.setText("关机计划已经启动");
        msgBox.setInformativeText("是否关闭关机计划");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
          case QMessageBox::Ok:
              on_cancelButton_clicked();
              //event->accept();
              break;
          case QMessageBox::Discard:
          case QMessageBox::Cancel:
          default:
            event->ignore();
              break;
        }
    }


}
