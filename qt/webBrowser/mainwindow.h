#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebKit/QtWebKit>
#include <QtWebKitWidgets/QWebView>
#include <QUrl>
#include <QTabWidget>
#include <QPushButton>

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

    void close_tab(int index);

    void on_titleButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
