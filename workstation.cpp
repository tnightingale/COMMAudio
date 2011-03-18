#include "workstation.h"
#include "mainwindow.h"
#include "tcpsocket.h"
#include "udpsocket.h"

Workstation::Workstation(MainWindow * mainWindow) {
    int err = 0;
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2,2);
    if ((err = WSAStartup(wVersionRequested, &wsaData)) < 0) {
        throw "TCPSocket::TCPSocket(): Missing WINSOCK2 DLL.";
    }

    tcpSocket_ = new TCPSocket(mainWindow->winId());
    udpSocket_ = new UDPSocket(mainWindow->winId());
}

Workstation::~Workstation() {
    delete tcpSocket_;
    delete udpSocket_;
}
