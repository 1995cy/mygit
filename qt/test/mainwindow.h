#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QAction>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QFileDevice>
//#include <QFileDialog>

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
    void on_pushButton_clicked();

    void on_actionAdd_bookmark_triggered();

   // void on_sender();


private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
