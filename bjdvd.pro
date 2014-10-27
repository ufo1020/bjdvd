#-------------------------------------------------
#
# Project created by QtCreator 2014-09-18T22:25:58
#
#-------------------------------------------------

QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bjdvd
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    page.cpp \
    networkAccess.cpp

HEADERS  += mainwindow.h \
    page.h \
    common.h \
    networkAccess.h

FORMS    += mainwindow.ui

CONFIG += mobility c++11
MOBILITY = 

