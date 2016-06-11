#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scri = new ScribbleArea();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::full_screen(){
    this->pmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    ui->label->setPixmap(pmap.scaled(ui->label->size()));
    this->show();
    this->time->stop();
}

void MainWindow::on_fullButton_clicked()
{
    this->hide();
    time = new QTimer();
    QObject::connect(time,SIGNAL(timeout()),this,SLOT(full_screen()));
    time->start(200);
}

void MainWindow::on_saveButton_clicked()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    str.append(".jpg");
    this->pmap.save(str,"JPG");
    QMessageBox::about(this,"成功","保存成功");
}

void MainWindow::on_someButton_clicked()
{
    this->hide();
    time = new QTimer();
    QObject::connect(time,SIGNAL(timeout()),this,SLOT(someScreen()));

    time->start(200);

}

void MainWindow::someScreen(){
    QObject::connect(scri,SIGNAL(screenGet()),this,SLOT(getSomeScreen()));
    scri->showFullScreen();
    scri->openImage();
    time->stop();
    this->show();
}

void MainWindow::getSomeScreen(){
    qDebug()<<"screen";
    this->pmap = scri->pixmap;
    ui->label->setPixmap(pmap.scaled(ui->label->size()));
}

void MainWindow::on_copyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(this->pmap);
    QMessageBox::about(this,"成功","已成功将图片复制到剪切板");

}
