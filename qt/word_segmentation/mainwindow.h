#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsql.h>
#include <QTextCodec>
#include <QDebug>
#include <QSqlQuery>
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
    void on_word_Button_clicked();

private:
    Ui::MainWindow *ui; 
    QSqlDatabase* con_sql();
    void close_sql(QSqlDatabase *db);
    bool find_word(QString word);

};

#endif // MAINWINDOW_H
