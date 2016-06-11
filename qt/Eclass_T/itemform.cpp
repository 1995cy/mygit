#include "itemform.h"
#include "ui_itemform.h"

itemForm::itemForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::itemForm)
{
    ui->setupUi(this);
    out.append("OUT");
    rev_mess(&out);
}

itemForm::~itemForm()
{
    delete ui;
}

void itemForm::init(QString name, QTcpSocket *tcpsock){
    this->setWindowTitle(name);
    this->name = name;
    this->sock = tcpsock;
}


void itemForm::on_outButton_clicked()
{
    this->sock->write(this->out);
    this->close();
}

void itemForm::rev_mess(QByteArray *mess){
    QByteArray *cash = new QByteArray(*mess);

    mess->clear();
    QDataStream out(mess,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out<<(quint64) 0;
    out<<*cash;
    out.device()->seek(0);
    out<<(quint64)(mess->size() - sizeof(quint64));
}

void itemForm::on_textButton_clicked()
{
    QByteArray m = ui->textEdit->toPlainText().toUtf8();
    if(m.isEmpty()){
        return;
    }
    QByteArray mess ="F_mess " + m;
    rev_mess(&mess);
    this->sock->write(mess);
   // ui->textButton->setText("");
}
