#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QDebug>
#include <QImage>
#include <QByteArray>
#include <QBuffer>
#include <QFileDialog>
#include <QFile>
#include <QFileDevice>
#include <QDataStream>
#include <QIODevice>
#include <QList>
#include <QMessageBox>
#include <QProcess>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork>
#include <QNetworkInterface>


#include "student_list.h"


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


    void screen_get();      //截屏

    void con_new();

    void close_sock();

    void data_read();

    void on_action_list_triggered();

    void on_action_file_triggered();

    void on_action_share_triggered();

    void on_action_server_triggered();

    void on_action_paint_triggered();

    void on_action_screen_triggered();

    void on_action_deskVideo_triggered();

    void on_action_help_triggered();

    void on_action_about_triggered();

private:
    void on_connButton_clicked();

    void on_shareButton_clicked();

    void on_fileButton_clicked();

    void on_paintButton_clicked();


    Ui::MainWindow *ui;
    bool con;               //是否已经连接服务器
    bool share;             //是否已经共享屏幕
    int outIime;
    qint64 blockSize;
    qint64 num;
    QByteArray nextFortune;
    QString title;

    QList<struct stu_im> Ssock;

    QPixmap pmap;       //截屏
    QTimer *timer;

    student_list *listview;

    QByteArray *byte;//桌面变量
    QByteArray *no;//当未共享桌面的时候返回的字符串
    QByteArray *out;//测试使用变量


    QTcpServer *tcpserver;

    QString getIPAddress();

    void what_do(QByteArray ca, QTcpSocket *sock);

    void rev_mess(QByteArray *mess);

};

#endif // MAINWINDOW_H
