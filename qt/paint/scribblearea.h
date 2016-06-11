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
#include <QFont>


class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = 0);
    bool isopen;
    QString what_do;
    QString text;

    bool openImage();
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setWordWidth(int newWidth);
    void ph_back();
    void re_do();

    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    int wordWidth() const { return myWordWidth; }

public slots:
    void clearImage();
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
    int myPenWidth;
    int myWordWidth;
    QColor myPenColor;
    QImage image;
    QPoint lastPoint;
    QList<QImage> list;
    int now;
};
//! [0]

#endif
