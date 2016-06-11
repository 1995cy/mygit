#-------------------------------------------------
#
# Project created by QtCreator 2016-05-17T20:55:59
#
#-------------------------------------------------

QT       += core gui
QT += printsupport
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = paint
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    scribblearea.cpp

HEADERS  += widget.h \
    scribblearea.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc
