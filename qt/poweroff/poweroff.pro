#-------------------------------------------------
#
# Project created by QtCreator 2016-03-14T20:59:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = poweroff
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    timedialog.cpp


HEADERS  += mainwindow.h \
    dialog.h \
    timedialog.h

FORMS    += mainwindow.ui \
    dialog.ui \
    timedialog.ui
