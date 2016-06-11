#include "s_main.h"
#include "ui_s_main.h"

S_Main::S_Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::S_Main)
{
    ui->setupUi(this);
    this->setWindowTitle("已连接到教师端");
    this->setFixedSize(500,150);
    remess(&this->hand,"S_hand");
    QObject::connect(this->tcpsock,SIGNAL(disconnected()),this,SLOT(discon()));
}

S_Main::~S_Main()
{
    delete ui;
}

void S_Main::on_fileButton_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(this,
                      "选择要发送的文件",QDir::currentPath());
        if(fileName.isEmpty()){//判断是否进行了操作
           return ;
        }
        //qDebug()<<fileName;
       int index = fileName.lastIndexOf('/') + 1;
       QByteArray name = fileName.mid(index).toUtf8();
       QByteArray Fsend;
       Fsend = "S_send ";
       name.append("!!!");
       Fsend.append(name);

       file = new QFile(fileName);
       if(!file->open(QFile::ReadOnly)){
            QMessageBox::critical(this,"error","该文件打开失败,请重试");
            return;
        }

       //filesize = file->size();

       QByteArray cash = file->readAll();
       Fsend.append(cash);

       //filesize = Fsend.size();
       cash.clear();

      remess(&cash,Fsend);

       qDebug()<< this->tcpsock->write(cash);
       file->close();
       QMessageBox::about(this,"succeed","file send succeed");
       //qDebug()<<name;

}

void S_Main::remess(QByteArray *cash, QByteArray Fsend){
    QDataStream out(cash,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out<<(quint64) 0;
    out<<Fsend;
    out.device()->seek(0);
    out<<(quint64)(cash->size() - sizeof(quint64));
}

void S_Main::init(QTcpSocket *sock, desktop_show *desk){
    this->tcpsock = sock;
    this->desktop = desk;
}

void S_Main::closeEvent(QCloseEvent *event){
    this->desktop->my_exit();
    event->accept();
}

void S_Main::on_handButton_clicked()
{
    this->tcpsock->write(this->hand);
}
