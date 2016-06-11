#ifndef LASTWINDOWS_H
#define LASTWINDOWS_H

#include <QMainWindow>

namespace Ui {
class lastWindows;
}

class lastWindows : public QMainWindow
{
    Q_OBJECT

public:
    explicit lastWindows(QWidget *parent = 0);
    ~lastWindows();

private slots:
    void on_pushButton_clicked();

private:
    Ui::lastWindows *ui;
};

#endif // LASTWINDOWS_H
