#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QColorDialog>
#include <QInputDialog>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QPalette>
#include <QString>

#include "scribblearea.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void penColor();
    void penWidth();

    void on_controlButton_clicked();

    void on_lineButton_clicked();

    void on_rectButton_clicked();

    void on_circularButton_clicked();

    void on_penButton_clicked();

    void on_backButton_clicked();

    void on_redoButton_clicked();

    void on_wordButton_clicked();

    void on_sizeWordButton_clicked();

private:
    Ui::Widget *ui;

    ScribbleArea *scribbleArea;

    void open_is();
};

#endif // WIDGET_H
