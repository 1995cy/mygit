#ifndef S_MAIN_H
#define S_MAIN_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QByteArray>
#include <QFile>
#include <QFileDevice>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDataStream>

#include "desktop_show.h"

namespace Ui {
class S_Main;
}

class S_Main : public QMainWindow
{
    Q_OBJECT

public:
    explicit S_Main(QWidget *parent = 0);
    ~S_Main();

    void init(QTcpSocket *sock,desktop_show *desk);

private slots:
    void on_fileButton_clicked();

    void on_handButton_clicked();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::S_Main *ui;

    QTcpSocket *tcpsock;

    desktop_show *desktop;

    QFile *file;
    qint64 filesize;
    QByteArray hand;

    void remess(QByteArray *cash,QByteArray Fsend);
};

#endif // S_MAIN_H
