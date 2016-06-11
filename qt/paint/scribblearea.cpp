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
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif

#include "scribblearea.h"

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
//   setWindowFlags(Qt::FramelessWindowHint);
    this->what_do = "PEN";
    scribbling = false;
    isopen = false;
    now = 0;
    myPenWidth = 5;
    myWordWidth = 15;
    myPenColor = Qt::red;
}

bool ScribbleArea::openImage()
{
    //QPixmap pmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    int w = QApplication::desktop()->availableGeometry(this).width();
    int h = QApplication::desktop()->availableGeometry(this).height();
    qDebug()<<w<<"c"<<h;
    QPixmap pmap = QPixmap::grabWindow(0,0,25,w);

       QImage loadedImage = (pmap.toImage());

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    list.clear();
    now = 1;
    list.append(image);
    update();
    return true;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::setWordWidth(int newWidth){
    myWordWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {
            lastPoint = event->pos();

           if(what_do != "TEXT"){

                scribbling = true;
           }

     }

}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
        if ((event->buttons() & Qt::LeftButton) && scribbling){
            if(what_do != "PEN")
            image = list.at((now - 1));
            drawLineTo(event->pos());
            update();
        }

}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton )

            if (scribbling) {

                scribbling = false;
                list = list.mid(0,now);
                list.append(image);
                now++;
            }else {
                list = list.mid(0,now);
            }

    drawLineTo(event->pos());
    update();
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
        this->clearImage();
        this->isopen = false;
    }
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    if(this->what_do == "PEN"){
        painter.drawLine(lastPoint, endPoint);
        lastPoint = endPoint;
    }else if(this->what_do == "LINE"){
        painter.drawLine(lastPoint, endPoint);
    }else if(what_do == "TEXT"){
        QFont font = painter.font();
        font.setPointSize(myWordWidth);
        painter.setFont(font);
        painter.drawText(lastPoint,text);
        list.append(image);
        now++;
    }else {
        QPointF *last = new QPointF(lastPoint);
        QPointF *end = new QPointF(endPoint);

        QRectF *graphic = new QRectF(*last,*end);
        if(this->what_do == "CIR"){
            painter.drawEllipse(*graphic);
        }else if(this->what_do == "RECT"){
            painter.drawRect(*graphic);
        }
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

void ScribbleArea::ph_back(){
    if(now > 1){
        //list.removeAt((list.size() - 1));
        now--;
        image = list.at((now - 1));
        update();
    }

}

void ScribbleArea::re_do(){
    if(now < list.size() && now > 0){
        now++;
        image = list.at((now - 1));
        update();
    }
}
