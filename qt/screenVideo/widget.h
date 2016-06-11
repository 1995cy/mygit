#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QProcess>
#include <QDateTime>
#include <QDesktopWidget>
#include <QCloseEvent>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_conButton_clicked();

    void on_saveButton_clicked();

 //   void on_sleepButton_clicked();

    void on_outButton_clicked();

private:
    Ui::Widget *ui;


    bool isDesk;
    QProcess *process;
    QString video_name;
    QString cmdLine;
    QString cmdF;
    QString resolution;
    int desk_x;
    int desk_y;
};

#endif // WIDGET_H
