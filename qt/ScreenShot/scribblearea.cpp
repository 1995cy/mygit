/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>

#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
//   setWindowFlags(Qt::FramelessWindowHint);
    scribbling = false;
    myPenWidth = 1;
    myPenColor = Qt::black;
    desk_x = QApplication::desktop()->width();
    desk_y = QApplication::desktop()->height();
}

bool ScribbleArea::openImage()
{
    QPixmap pmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    /*int w = QApplication::desktop()->availableGeometry(this).width();
    int h = QApplication::desktop()->availableGeometry(this).height();
    qDebug()<<w<<"c"<<h;
    QPixmap pmap = QPixmap::grabWindow(0,0,25,w);*/

       QImage loadedImage = (pmap.toImage());

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    firstImage = image;
    first = true;
    update();
drawLineTo(lastPoint);
    return true;
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {
            lastPoint = event->pos();
            last = new QPointF(lastPoint);
            scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{

    if ((event->buttons() & Qt::LeftButton) && scribbling){
        image = firstImage;
        update();
           drawLineTo(event->pos());
     }


}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton )
            if (scribbling) {
                drawLineTo(event->pos());
             scribbling = false;
             QRect *rect = new QRect(lastPoint,event->pos());
             pixmap = QPixmap::grabWidget(this,*rect);
             //pixmap = QPixmap::grabWindow(sm_x,sm_y,(big_y - sm_y),(big_x - sm_x));
             emit screenGet();
             this->hide();
            }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Escape){
        this->hide();
    }
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
     if(first){
        painter.setBrush(QBrush(Qt::black, Qt::Dense4Pattern));
        painter.drawRect(0,0,desk_x,desk_y);
        first = false;
     }else {
         QPointF *end = new QPointF(endPoint);
         QRectF *graphic = new QRectF(*last,*end);
         painter.drawRect(*graphic);
         painter.setBrush(QBrush(Qt::black, Qt::Dense4Pattern));
          big_y = (lastPoint.y() < endPoint.y() ?endPoint.y():lastPoint.y());
          big_x = (lastPoint.x() < endPoint.x() ?endPoint.x():lastPoint.x());
          sm_y = (lastPoint.y() > endPoint.y() ?endPoint.y():lastPoint.y());
          sm_x = (lastPoint.x() > endPoint.x() ?endPoint.x():lastPoint.x());
         painter.drawRect(0,0,desk_x,sm_y);

         painter.drawRect(0,big_y,desk_x,(desk_y - big_y));
         painter.drawRect(0,0,sm_x,desk_y);
         painter.drawRect(big_x,0,(desk_x - big_x),desk_y);
     }



    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_NO_PRINTER
}
