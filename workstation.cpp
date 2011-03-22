#include "workstation.h"
#include "mainwindow.h"
#include "tcpsocket.h"
#include "udpsocket.h"

Workstation::Workstation(MainWindow *mainWindow) {
    int err = 0;
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2,2);

    if ((err = WSAStartup(wVersionRequested, &wsaData)) < 0) {
        qDebug("Workstation::Workstation(): Missing WINSOCK2 DLL.");
        throw "Workstation::Workstation(): Missing WINSOCK2 DLL.";
    }

    // Create TCP socket to listen for requests
    tcpSocket_ = new TCPSocket(mainWindow->winId());

    // Connect signal and slot for WSA events
    connect(mainWindow, SIGNAL(signalWMWSASyncTCPRx(PMSG)),
            tcpSocket_, SLOT(slotProcessWSAEvent(PMSG)));

    // Connect signal and slot for processing a new connection
    connect(tcpSocket_, SIGNAL(signalClientConnected(TCPSocket*)),
            this, SLOT(processConnection(TCPSocket*)));

    udpSocket_ = new UDPSocket(mainWindow->winId());
    connect(mainWindow, SIGNAL(signalWMWSASyncUDPRx(PMSG)),
            udpSocket_, SLOT(slotProcessWSAEvent(PMSG)));

    tcpSocket_->listen(7000);
}

Workstation::~Workstation() {
    delete tcpSocket_;
    delete udpSocket_;
}

void Workstation::requestFileList()
{

}

void Workstation::sendFile()
{

}

void Workstation::sendFileList()
{

}

void Workstation::acceptVoiceChat()
{

}

void Workstation::connectToServer()
{

}

void Workstation::requestFile()
{

}

/*
-- FUNCTION: processConnection
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: void Workstation::processConnection();
--
-- RETURNS: void
--
-- NOTES:
-- The Qt slot function that gets called from the listening socket. This
-- function will call the correct private function based on the type of control
-- message received.
*/
void Workstation::processConnection(TCPSocket* socket)
{

}

void Workstation::receiveUDP()
{

}

void Workstation::receiveFile()
{

}
