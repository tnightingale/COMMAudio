#-------------------------------------------------
#
# Project created by QtCreator 2011-03-07T11:12:09
#
#-------------------------------------------------

QT       += core gui \
            phonon \
            multimedia

TARGET    = COMMAudio
TEMPLATE  = app
CONFIG   += qt debug

SOURCES  += main.cpp \
            mainwindow.cpp \
            socket.cpp \
            tcpsocket.cpp \
            udpsocket.cpp \
            workstation.cpp\
            audiocomponent.cpp

HEADERS  += mainwindow.h \
            socket.h \
            tcpsocket.h \
            udpsocket.h \
            workstation.h\
            audiocomponent.h

FORMS    += mainwindow.ui

INCLUDEPATH += "C:/OpenAL 1.1 SDK/include"

LIBS += C:/Qt/2010.05/mingw/lib/libws2_32.a
LIBS += "C:/OpenAL 1.1 SDK/libs/Win32/OpenAL32.lib"
