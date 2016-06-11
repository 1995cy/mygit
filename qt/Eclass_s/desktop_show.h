#ifndef DESKTOP_SHOW_H
#define DESKTOP_SHOW_H

#include <QWidget>
#include <QLabel>
#include <QByteArray>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class desktop_show;
}

class desktop_show : public QWidget
{
    Q_OBJECT

public:
    explicit desktop_show(QWidget *parent = 0);
    ~desktop_show();

    void show_des(QByteArray photo);

    void do_not();

    void my_exit();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::desktop_show *ui;

    QByteArray photo;

    bool out;

};

#endif // DESKTOP_SHOW_H
