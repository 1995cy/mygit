#ifndef ITEMFORM_H
#define ITEMFORM_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>

namespace Ui {
class itemForm;
}

class itemForm : public QWidget
{
    Q_OBJECT

public:
    explicit itemForm(QWidget *parent = 0);
    ~itemForm();

    void init(QString name,QTcpSocket *tcpsock);

private slots:
    void on_outButton_clicked();

    void on_textButton_clicked();

private:
    Ui::itemForm *ui;

    QTcpSocket *sock;
    QString name;
    QByteArray out;

    void rev_mess(QByteArray *mess);
};

#endif // ITEMFORM_H
