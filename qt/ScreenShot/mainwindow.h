#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QDesktopWidget>
#include <QTimer>
#include <QDateTime>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QClipboard>

#include "scribblearea.h"

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
    void on_fullButton_clicked();

    void full_screen();

    void on_saveButton_clicked();

    void on_someButton_clicked();

    void on_copyButton_clicked();

    void someScreen();

    void getSomeScreen();

private:
    Ui::MainWindow *ui;

    QPixmap pmap;
    QTimer *time;

    ScribbleArea *scri;
};

#endif // MAINWINDOW_H
