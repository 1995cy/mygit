#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T21:03:38
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Eclass_s
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    s_main.cpp \
    desktop_show.cpp

HEADERS  += mainwindow.h \
    s_main.h \
    desktop_show.h

FORMS    += mainwindow.ui \
    s_main.ui \
    desktop_show.ui

RESOURCES += \
    image.qrc
