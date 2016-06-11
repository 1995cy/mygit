#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("运行");
    //w.setStyleSheet("color:white");
    w.show();

    return a.exec();
}
