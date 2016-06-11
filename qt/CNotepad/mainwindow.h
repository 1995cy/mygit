#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileDevice>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QColor>
#include <QColorDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void saveFile(QString fileName);
    void isSave(bool isnew);
    void clear_new();

signals://自定义信号

private slots://槽

    void on_textEdit_textChanged();
    void on_new();
    void on_exit();
    void on_save();
    void on_help();
    void on_about();
    void on_open();
    void on_Asave();
    void on_font();
    void on_color();
};

#endif // MAINWINDOW_H
