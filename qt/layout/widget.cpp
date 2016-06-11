#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMaximumSize(369,191);
    this->setMinimumSize(369,191);

    //将信号和槽连接起来
    QObject::connect(ui->inputEdit,SIGNAL(returnPressed())
                     ,this,SLOT(on_pushButton_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString programName = ui->inputEdit->text();//获取文本框的字符串
    QProcess *process = new QProcess;//创建进程
    process->start(programName.trimmed());//打开相应的进程

    this->close();//关闭该进程
}

void Widget::on_pushButton_2_clicked()
{
    this->close();//关闭该进程
}
