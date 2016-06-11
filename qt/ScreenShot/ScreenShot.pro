#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T17:10:25
#
#-------------------------------------------------

QT       += core gui
QT  += widgets
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScreenShot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scribblearea.cpp

HEADERS  += mainwindow.h \
    scribblearea.h

FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    image.qrc
