#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QEvent>
#include <QString>
#include <QPointF>
#include <QRectF>
#include <QBrush>
#include <QRect>


class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);

    bool openImage();
    QPixmap pixmap;

signals:
    void screenGet();

public slots:
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event);

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    bool scribbling;
    bool first;
    int myPenWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;
    QPointF *last;

    QImage firstImage;
    int desk_x;
    int desk_y;

    int big_y;
    int big_x;
    int sm_y;
    int sm_x;
};
//! [0]

#endif
