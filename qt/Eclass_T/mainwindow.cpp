#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helpdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(500,310);
    //初始化状态
    listview = new student_list;
    this->byte = new QByteArray;
    this->out = new QByteArray("OUT");
    rev_mess(this->out);
    title = "机房教学----教师端";

    this->share = false;
    this->con = false;
    this->blockSize = 0;
    this->num = 0;
    this->outIime = ((1 << 31) - 1);
    this->no = new QByteArray("NOT");
    rev_mess(this->no);
    ui->statusBar->showMessage("服务未打开",this->outIime);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//点击连接按键
void MainWindow::on_connButton_clicked()
{
    if(this->con == false){
        this->tcpserver = new QTcpServer(this);

        if(!tcpserver->listen(QHostAddress::Any,8888))
           {

              qDebug()<<tcpserver->errorString();
              tcpserver->close();
              return;
           }
        QObject::connect(this->tcpserver,SIGNAL(newConnection()),this,SLOT(con_new()));
        this->con = true;
        QString sho = "服务已打开,IP地址为: " + getIPAddress();

        ui->statusBar->showMessage(sho,this->outIime);
        sho = title + sho;
        this->setWindowTitle(sho);
        //listview->show();
    }else {
        this->con = false;
        for(int i = 0;i < Ssock.size();i++){
            Ssock.at(i).sock->write(*this->out);
        }
        this->tcpserver->close();
        ui->statusBar->showMessage("服务未打开",this->outIime);
        this->setWindowTitle(title);
        listview->hide();
    }
}

//新的连接
void MainWindow::con_new(){
    QTcpSocket *tcpsock = this->tcpserver->nextPendingConnection();
    qDebug()<<"new connect";
    QObject::connect(tcpsock,SIGNAL(disconnected()),this,SLOT(close_sock()));
    QObject::connect(tcpsock,SIGNAL(readyRead()),this,SLOT(data_read()));

}

//关闭套接字
void MainWindow::close_sock(){
    QTcpSocket *s =(QTcpSocket *) QObject::sender();
    s->close();
    for(int i = 0;i < Ssock.count();i++){
        if(!Ssock.at(i).sock->isOpen()){
            listview->list_del(Ssock.at(i).stu);
        }
    }
    qDebug()<<"disconnect";
}

//读取数据
void MainWindow::data_read(){
    QTcpSocket *s =(QTcpSocket *) QObject::sender();

    if(this->blockSize == 0){
        QDataStream out(s);
        out.setVersion(QDataStream::Qt_4_0);
        out >> blockSize;
        qDebug()<<"block"<<blockSize;
        num = 0;
        nextFortune.clear();
    }

    if(s->bytesAvailable() > 0){
        qint64 len = s->bytesAvailable();

        qint64 ca = blockSize - num;
        QByteArray cash;
        if(len > ca){
            cash = s->read(ca);
            num = blockSize;
        }else {
           cash = s->readAll();
           num +=len;
        }

        nextFortune.append(cash);
   }

    if(num >= blockSize){
        what_do(nextFortune,s);
        blockSize = 0;
    }


}

//处理请求
void MainWindow::what_do(QByteArray ca, QTcpSocket *sock){
    QByteArray mess = ca.mid(4);
   if(mess.startsWith("S_login ")){
        QString left = mess.mid(8);
        struct stu_im one_student;
        one_student.sock = sock;
        one_student.stu = left;
        qDebug()<<left;
        Ssock.append(one_student);
        listview->list_add(left,sock);
    }else if(mess.startsWith("S_send ")){
        QByteArray left = mess.mid(7);

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

    }else if(mess.startsWith("S_hand")){
       for(int i = 0;i < Ssock.size();i++){
           if(sock == Ssock[i].sock){
               QString han = Ssock.at(i).stu + "举手提问";
               QMessageBox::about(this,"举手",han);
               break;
           }
       }
    }
   else{
qDebug()<<"unknow";
   }
}

//点击共享按键
void MainWindow::on_shareButton_clicked()
{
        if(this->share){
            this->timer->stop();
            QObject::disconnect(this->timer,SIGNAL(timeout()),this,SLOT(screen_get()));


            for(int i = 0;i < Ssock.size();i++){
                Ssock.at(i).sock->write(*this->no);
            }
            share = false;
            ui->label->setText("桌面未共享");
        }else{
             this->timer = new QTimer;
             QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT(screen_get()));
             this->timer->start(250);
             share = true;
             ui->label->setText("桌面已共享,但无学生端");
        }

}

//截屏
void MainWindow::screen_get(){
    if(Ssock.size() > 0){
        this->pmap = QPixmap::grabWindow(QApplication::desktop()->winId());

        QImage image = (this->pmap.toImage());

        this->byte->clear();
        QBuffer buffer(this->byte);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "JPG");


        QPixmap *map = new QPixmap;
        map->loadFromData(*byte,(const char *)0,Qt::AutoColor);
        ui->label->setPixmap(map->scaled(ui->label->size()));

        QByteArray *cash =new QByteArray(*this->byte);
        this->byte = new QByteArray("F_Desktop ");
        this->byte->append(*cash);
        if(this->byte->startsWith("F_Desktop ")){
            qDebug()<<this->byte->size();
        }


        rev_mess(this->byte);

        for(int i = 0;i < Ssock.size();i++){
            Ssock.at(i).sock->write(*this->byte);
        }
    }

}

//对要发送的数据进行处理
void MainWindow::rev_mess(QByteArray *mess){
    QByteArray *cash = new QByteArray(*mess);

    mess->clear();
    QDataStream out(mess,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out<<(quint64) 0;
    out<<*cash;
    out.device()->seek(0);
    out<<(quint64)(mess->size() - sizeof(quint64));
}

//点击传输文件按键
void MainWindow::on_fileButton_clicked()
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
       Fsend = "F_file ";
       name.append("!!!");
       Fsend.append(name);

      QFile *file = new QFile(fileName);
       if(!file->open(QFile::ReadOnly)){
            QMessageBox::critical(this,"error","该文件打开失败,请重试");
            return;
        }

       //filesize = file->size();

       QByteArray cash = file->readAll();
       Fsend.append(cash);

       //filesize = Fsend.size();
       cash.clear();

       QDataStream out(&cash,QIODevice::WriteOnly);
       out.setVersion(QDataStream::Qt_4_0);
       out<<(quint64) 0;
       out<<Fsend;
       out.device()->seek(0);
       out<<(quint64)(cash.size() - sizeof(quint64));

       for(int i = 0;i < Ssock.size();i++){
           Ssock.at(i).sock->write(cash);
       }
QMessageBox::about(this,"succeed","file send ok");
       file->close();
       //qDebug()<<name;

}

//获取到IP地址
QString MainWindow::getIPAddress(){
    QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
                ipAddress = ipAddressesList.at(i).toString();
               // break;
            }
        }
        if (ipAddress.isEmpty())
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
        return ipAddress;
}

//学生列表
void MainWindow::on_action_list_triggered()
{
    listview->show();
}

void MainWindow::on_action_file_triggered()
{
    on_fileButton_clicked();
}

void MainWindow::on_action_share_triggered()
{
    on_shareButton_clicked();
}

void MainWindow::on_action_server_triggered()
{
    on_connButton_clicked();
}

void MainWindow::on_paintButton_clicked()
{
    QProcess *process = new QProcess;
    process->start("./paint");
}

void MainWindow::on_action_paint_triggered()
{
    on_paintButton_clicked();
}

void MainWindow::on_action_screen_triggered()
{
    QProcess *process = new QProcess;
    process->start("./ScreenShot");
}

void MainWindow::on_action_deskVideo_triggered()
{
    QProcess *process = new QProcess;
    process->start("./screenVideo");
}

void MainWindow::on_action_help_triggered()
{
    helpDialog *help_dialog = new helpDialog;
    help_dialog->show();
    help_dialog->exec();

}

void MainWindow::on_action_about_triggered()
{
    QMessageBox::about(this,"关于","版本:1.0\n开发:C_H团队\n任何吐槽和bug请联系我们:827170272@qq.com");
}
