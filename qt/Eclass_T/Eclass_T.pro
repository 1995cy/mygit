#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T08:51:37
#
#-------------------------------------------------

QT       += core gui
QT  += network
#QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Eclass_T
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    student_list.cpp \
    itemform.cpp \
    helpdialog.cpp

HEADERS  += mainwindow.h \
    student_list.h \
    itemform.h \
    helpdialog.h

FORMS    += mainwindow.ui \
    student_list.ui \
    itemform.ui \
    helpdialog.ui

RESOURCES += \
    image.qrc
