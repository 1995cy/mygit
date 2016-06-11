#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setPort(3308);
        db.setDatabaseName("studentcourserj0212");
        db.setUserName("root");
        db.setPassword("123");
        bool ok = db.open();
        if(ok){
            qDebug()<<"open successfuly";
            db.close();
            db.removeDatabase("studentcourserj0212");
        }else{
            qDebug()<<"failed"<<db.lastError().text();
        }

}
