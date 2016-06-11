#include "desktop_show.h"
#include "ui_desktop_show.h"

desktop_show::desktop_show(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::desktop_show)
{
    ui->setupUi(this);
    ui->label->setText("教师未共享桌面");
    this->out = false;
}



desktop_show::~desktop_show()
{
    delete ui;
}

void desktop_show::my_exit(){
    this->out = true;
    this->close();
}

void desktop_show::closeEvent(QCloseEvent *event){
    if(out == true){
        event->accept();
    }else {
        event->ignore();
        QMessageBox::critical(this,"error","桌面显示窗口不能关闭");
    }

}

void desktop_show::show_des(QByteArray photo){
    QPixmap *map = new QPixmap;
    map->loadFromData(photo,(const char *)0,Qt::AutoColor);
    ui->label->setPixmap(map->scaled(ui->label->size()));
}

void desktop_show::do_not(){
    ui->label->setText("教师未共享桌面");
}
