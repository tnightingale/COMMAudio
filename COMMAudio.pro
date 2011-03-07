#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T11:12:09
#
#-------------------------------------------------

QT       += core gui

TARGET	  = COMMAudio
TEMPLATE  = app


SOURCES	 += main.cpp \
            mainwindow.cpp \
            socket.cpp \
            tcpsocket.cpp \
            udpsocket.cpp

HEADERS  += mainwindow.h \
            socket.h \
            tcpsocket.h \
            udpsocket.h

FORMS    += mainwindow.ui
