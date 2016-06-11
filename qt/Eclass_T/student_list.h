#ifndef STUDENT_LIST_H
#define STUDENT_LIST_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QIcon>
#include <QList>
#include <QTcpSocket>


#include "itemform.h"

struct stu_im{
    QString stu;
    QTcpSocket *sock;
};

namespace Ui {
class student_list;
}

class student_list : public QMainWindow
{
    Q_OBJECT

public:
    explicit student_list(QWidget *parent = 0);
    ~student_list();

    //add list for students who connects this
    void list_add(QString mess, QTcpSocket *soc);

    //delete list
    void list_del(QString mess);

private slots:
    void click_item(QListWidgetItem*item);

private:
    Ui::student_list *ui;

    QList<struct stu_im> Ssock;

    itemForm form_item;
};

#endif // STUDENT_LIST_H
