#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("简陋版记事本\ ");

    return a.exec();
}
