#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QDebug>
#include <stdlib.h>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_dateButton_clicked();

    void on_timeButton_clicked();

    void on_cancelButton_clicked();

    void showTimeDown();

private:
    Ui::MainWindow *ui;
    void countdownDO();
};

#endif // MAINWINDOW_H
