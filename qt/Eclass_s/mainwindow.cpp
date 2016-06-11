#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->sock = new QTcpSocket(this);
    student = new S_Main;

    this->blockSize = 0;
    this->num = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadButton_clicked()
{
    QString ip = ui->IPEdit->text();
    if(ip.isEmpty()){
        QMessageBox::critical(this,"error","请输入IP地址");
        return;
    }
    QByteArray id_name = ui->nameEdit->text().toUtf8().append("\n") + ui->idEdit->text().toUtf8();
    if(id_name.isEmpty()){
        QMessageBox::critical(this,"error","请输入姓名或者学号");
        return;
    }
//QByteArray id_name = "huangjiahao\n2013";
     QObject::connect(this->sock,SIGNAL(error(QAbstractSocket::SocketError))
                      ,
                      this,SLOT(er_con(QAbstractSocket::SocketError)));
     QObject::connect(this->sock,SIGNAL(readyRead()),this,SLOT(readData()));
     QObject::connect(this->sock,SIGNAL(hostFound()),this,SLOT(find_host()));


     id_name = "S_login " + id_name;

     re_mess(id_name,&this->stuMess);
     this->sock->connectToHost(ip,8888);
    // this->sock->connectToHost("192.168.1.109",8888);

}

void MainWindow::readData(){

    if(this->blockSize == 0){
        QDataStream out(this->sock);
        out.setVersion(QDataStream::Qt_4_0);
        out >> blockSize;

        qDebug()<<"block"<<blockSize;
        num = 0;
        nextFortune.clear();
    }

    if(this->sock->bytesAvailable() > 0){
        qint64 len = this->sock->bytesAvailable();
        //num +=len;

        QByteArray cash;
        qint64 ca = blockSize - num;
        if(len > ca){
           cash = this->sock->read(ca);
           num += blockSize;
        }else {
            cash = this->sock->readAll();
            num += len;
        }

        nextFortune.append(cash);
   }

    if(num >= blockSize){
        qDebug()<<"size"<<nextFortune.size();
        what_do(nextFortune);
        blockSize = 0;
    }

}

void MainWindow::what_do(QByteArray mess){
    QByteArray b = mess.mid(4);
    if(b.startsWith("F_Desktop ")){
        qDebug()<<"desktop";

        QByteArray byte = b.mid(10);

       // this->desktop->show();
        this->desktop->showFullScreen();

        desktop->show_des(byte);
        qDebug()<<"desk";
    }else if(b == "NOT"){
        qDebug()<<"NOT";
        this->desktop->hide();

    }else if(b.startsWith("F_file ")){
        QByteArray left = b.mid(7);

        int index = left.indexOf("!!!");
        QByteArray name = left.mid(0,index);

        index += 3;
        QByteArray conntext = left.mid(index);

        QFile *writeFile = new QFile((QString)name);
        if(!writeFile->open(QFile::WriteOnly)){
            return;
        }
        writeFile->write(conntext);
        writeFile->close();
        QMessageBox::about(this,"succeed","new file get");
    }else if(b.startsWith("OUT")){
        this->student->close();
        this->close();
    }else if(b.startsWith("F_mess ")){
        QByteArray left = b.mid(7);

        QMessageBox::about(this,"老师向你发送了新消息",(QString)left);
    }

     else{
        qDebug()<<"unknow";
    }

}

void MainWindow::re_mess(QByteArray f_mess, QByteArray *cash){
    QDataStream out(cash,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
        out<<(quint64) 0;
        out<<f_mess;
        out.device()->seek(0);
        out<<(quint64)(cash->size() - sizeof(quint64));
}

void MainWindow::er_con(QAbstractSocket::SocketError){
    QMessageBox::critical(this,"连接出错","连接出错,请检查IP,网络和教师端服务是否打开");
    qDebug()<<this->sock->errorString();
    this->sock->close();
    this->close();
    this->student->close();
}

void MainWindow::on_cancelButton_clicked()
{
    ui->IPEdit->setText("");
    ui->idEdit->setText("");
    ui->nameEdit->setText("");
}

void MainWindow::find_host(){
    if(this->sock->isOpen()){
        this->sock->write(this->stuMess);
        qDebug()<<"start";
        desktop = new desktop_show;
        //desktop->show();
        student->init(this->sock,desktop);
        this->hide();
        student->show();
    }
}
