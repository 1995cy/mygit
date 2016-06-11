#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDataStream>
#include <QIODevice>
#include <QString>
#include <QTimer>


#include "s_main.h"
#include "desktop_show.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadButton_clicked();

    void on_cancelButton_clicked();

    void er_con(QAbstractSocket::SocketError);

    void readData();

    void find_host();

private:
    Ui::MainWindow *ui;

    QTcpSocket *sock;
    QTimer *time;
    //QByteArray *deskMess;
    QByteArray stuMess;

    qint64 blockSize;
    qint64 num;
    QByteArray nextFortune;

    S_Main *student;

    desktop_show *desktop;

    void re_mess(QByteArray f_mess, QByteArray *mess);

    void what_do(QByteArray mess);

};

#endif // MAINWINDOW_H
