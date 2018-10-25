#-------------------------------------------------
#
# Project created by QtCreator 2018-10-24T19:37:06
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mypoolthread
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    threadhandle.cpp \
    tcpsocket.cpp \
    tcpserver.cpp

HEADERS  += widget.h \
    threadhandle.h \
    tcpsocket.h \
    tcpserver.h

FORMS    += widget.ui
