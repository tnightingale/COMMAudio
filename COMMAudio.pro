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
            buffer.cpp \
            joinserver.cpp \
            workstation.cpp\
            audiocomponent.cpp \
            remoteSong.cpp \
            filedata.cpp \
            colors.cpp \
            multicastsession.cpp \
            downloads.cpp \
            hostmulticast.cpp \
            joinmulticast.cpp


HEADERS  += mainwindow.h \
            socket.h \
            tcpsocket.h \
            udpsocket.h \
            buffer.h \
            joinserver.h \
            workstation.h\
            audiocomponent.h \
            remoteSong.h \
            filedata.h \
            colors.h \
            multicastsession.h \
            downloads.h \
            hostmulticast.h \
            joinmulticast.h


FORMS    += mainwindow.ui \
            joinServer.ui \
            colors.ui \
            downloads.ui \
            hostmulticast.ui \
            joinmulticast.ui

LIBS     += C:/Qt/2010.05/mingw/lib/libws2_32.a
