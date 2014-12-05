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
    cookiejar.cpp \
    datamanager.cpp \
    userdata.cpp \
    networkaccess.cpp

HEADERS  += mainwindow.h \
    page.h \
    common.h \
    cookiejar.h \
    datamanager.h \
    userdata.h \
    networkaccess.h

FORMS    += mainwindow.ui \
    login.ui

CONFIG += mobility c++11
MOBILITY = 

