#include "student_list.h"
#include "ui_student_list.h"

student_list::student_list(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::student_list)
{
    ui->setupUi(this);
}

student_list::~student_list()
{
    delete ui;
}

void student_list::list_add(QString mess,QTcpSocket *soc){
    struct stu_im one_student;
    one_student.sock = soc;
    one_student.stu = mess;
    Ssock.append(one_student);

    QListWidgetItem *list = new QListWidgetItem;
    list->setText(mess);
    list->setIcon(*(new QIcon(":/new/prefix1/image/computer.jpg")));

    QObject::connect(ui->list_stu,SIGNAL(itemClicked(QListWidgetItem*)),
                     this,SLOT(click_item(QListWidgetItem*)));

    ui->list_stu->addItem(list);
}

void student_list::list_del(QString mess){
    QListWidgetItem *list = ui->list_stu->findItems(mess,Qt::MatchStartsWith).value(0);
    delete list;
}
void student_list::click_item(QListWidgetItem *item){
    for(int i = 0;i < this->Ssock.size();i++){

        if(this->Ssock.at(i).stu == item->text()){
               form_item.init(Ssock.at(i).stu,Ssock.at(i).sock);
               form_item.show();
               break;
        }
    }

}
