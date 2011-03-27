#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T11:12:09
#
#-------------------------------------------------

QT       += core gui \
            phonon \
            multimedia
CONFIG   += mobility
MOBILITY += multimedia

TARGET    = COMMAudio
TEMPLATE  = app
CONFIG   += qt debug

SOURCES  += main.cpp \
            mainwindow.cpp \
            socket.cpp \
            tcpsocket.cpp \
            udpsocket.cpp \
            joinserver.cpp \
            workstation.cpp\
            audiocomponent.cpp \
            remoteSong.cpp

HEADERS  += mainwindow.h \
            socket.h \
            tcpsocket.h \
            udpsocket.h \
            joinserver.h \
            workstation.h\
            audiocomponent.h \
            remoteSong.h

FORMS    += mainwindow.ui \
            joinServer.ui

LIBS     += C:/Qt/2010.05/mingw/lib/libws2_32.a
