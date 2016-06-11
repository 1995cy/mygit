#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));

    MainWindow w;
    w.show();
    w.setWindowTitle("你好");
    qApp->beep();
    return a.exec();
}
