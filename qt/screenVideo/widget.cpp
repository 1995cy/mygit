#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(400,110);
    isDesk = false;
    ui->saveButton->setEnabled(false);
    ui->outButton->setEnabled(false);

    desk_x = QApplication::desktop()->width();
    desk_y = QApplication::desktop()->height();

    this->resolution = QString::number(desk_x,10) + "x";
    this->resolution.append(QString::number(desk_y,10));
    this->cmdF = "ffmpeg -f x11grab -s " + this->resolution;
    this->cmdF.append(" -r 25 -i :0.0 ./");
}

Widget::~Widget()
{
    this->process->close();
    delete ui;
}


void Widget::on_conButton_clicked()
{
    if(isDesk == false){
        ui->saveButton->setEnabled(true);
        ui->outButton->setEnabled(true);
        ui->conButton->setEnabled(false);

        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
        this->video_name = str.append(".mp4");
        this->video_name = this->video_name.replace(" ",".");


        this->cmdLine = this->cmdF + this->video_name;
        qDebug()<<this->cmdLine;
        this->process = new QProcess(this);
        this->process->start(this->cmdLine);
        this->isDesk = true;
    }
}

void Widget::on_saveButton_clicked()
{
    this->process = new QProcess(this);
    this->process->start("./close");
    this->process->waitForFinished(500);

    ui->saveButton->setEnabled(false);
    ui->outButton->setEnabled(false);
    ui->conButton->setEnabled(true);

    this->isDesk = false;
}

void Widget::on_outButton_clicked()
{
    this->process->close();
    this->process = new QProcess(this);
    QString n = "rm " + this->video_name;
    process->start(n);
    process->waitForFinished(500);

    ui->saveButton->setEnabled(false);
    ui->outButton->setEnabled(false);
    ui->conButton->setEnabled(true);

    this->isDesk = false;
}

void Widget::closeEvent(QCloseEvent *event){
    if(!this->isDesk){
        event->accept();
    }else {
        int ok = QMessageBox::warning(this,"警告","桌面录制正在进行中,退出文件将不保存",
                             (QMessageBox::Discard | QMessageBox::Cancel));
        switch (ok) {
        case QMessageBox::Discard:
            on_outButton_clicked();
            event->accept();
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }
    }
}
