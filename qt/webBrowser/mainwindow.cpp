#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

int i = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->tabWidget,
                     SIGNAL(tabBarDoubleClicked(int)),
                     this,SLOT(close_tab(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QPushButton *button = new QPushButton;

    ui->tabWidget->addTab(button,QString::number(i));
    i++;
}

void MainWindow::close_tab(int index){
    ui->tabWidget->removeTab(index);
}

void MainWindow::on_titleButton_clicked()
{
   qDebug()<< ui->titleButton->parentWidget()->close();
}
