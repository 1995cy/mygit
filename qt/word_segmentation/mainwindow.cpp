#include "mainwindow.h"
#include "ui_mainwindow.h"

QString Punctuation[] = {" ",",",".",";","/","\\","=","\"","'",":"};
int len = 10;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_word_Button_clicked()
{
    /**
     *  i 		计数,如果超过5个字都没有找到的,就将最前面的字向后挪
     * now 		指向当前读取到的字的位置
     * head		当前需要匹配的字符串最开始的字的位置
     *		当匹配到了以后再继续匹配,指导找到最大的匹配为止
     * oldF 存储之前匹配到的头位置
     * oldB	存储之前匹配到的尾位置
     */
    int i = 0,now = 1,head = 0,oldB = 0,oldF = 0,m = 0;
    QString word,cash,text_showed;



    QString text = ui->textEdit->toPlainText();
    if(text == ""){
       const QString er = "请输入想要分词的文字";
        qDebug()<<er;
       ui->text_show->setText(er);
    }

    QSqlDatabase *db = con_sql();
    if(db == NULL){
        ui->text_show->setText("所需的数据库打开失败");
        return;
    }
   while(now <= text.length()){
       m = 0;
        if(cash == " "){
          text_showed += " ";
          text_showed += word;

          word = "";
          cash = "";
          i = 1;
          head = now;
          now++;
        }else{
            cash = text.mid((now - 1),1);

            while(m < len){
                if(cash == Punctuation[m]){
                    text_showed += " ";
                    text_showed += word;

                    word = "";
                    cash = "";
                    i = 1;
                    head = now;
                    now++;
                    break;
                }
                m++;
            }
            if(m != len){
                continue;
            }
            word += cash;

            //qDebug()<<cash;

                if(find_word(word)){
                    oldB = head;
                    oldF = now;
                }
                if(i == 7 || now == text.length()){
                    if(oldF != 0){
                        word = text.mid(oldB,(oldF - oldB));
                         cash = " ";
                          now = oldF - 1;
                          oldB = 0;
                          oldF = 0;
                    }
                }
            now++;
            i++;
        }
    }

   text_showed += " ";
   text_showed += word;

   ui->text_show->setText(text_showed);

   close_sql(db);
}

bool MainWindow::find_word(QString word){
    QString sql = "select id from chinaword where word='" + word;
    sql += "'";

    qDebug()<<word;

    QSqlQuery query;
    query.exec(sql);
    if(query.next()){
        return true;
    }else{
       return false;
    }
}

QSqlDatabase *MainWindow::con_sql(){
   static QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3308);
    db.setDatabaseName("test");
    db.setUserName("root");
    db.setPassword("123");
    bool ok = db.open();
    if(ok){
        qDebug()<<"open successfuly";
        return &db;
         }else{
           qDebug()<<"failed"<<db.lastError().text();
           return NULL;
         }
}

void MainWindow::close_sql(QSqlDatabase *db){
    db->close();
    db->removeDatabase("test");
    qDebug()<<"closed";
}
