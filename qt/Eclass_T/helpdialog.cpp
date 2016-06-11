#include "helpdialog.h"
#include "ui_helpdialog.h"

helpDialog::helpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::helpDialog)
{
    ui->setupUi(this);
    QMovie *move = new QMovie(":/new/prefix1/image/xiaohuangren.gif");

    ui->label_gif->setMovie(move);
    move->start();

}

helpDialog::~helpDialog()
{
    delete ui;
}
