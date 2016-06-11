#include "lastwindows.h"
#include "ui_lastwindows.h"

lastWindows::lastWindows(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::lastWindows)
{
    ui->setupUi(this);
}

lastWindows::~lastWindows()
{
    delete ui;
}

void lastWindows::on_pushButton_clicked()
{
    lastWindows *last = new lastWindows;
    last->show();
}
