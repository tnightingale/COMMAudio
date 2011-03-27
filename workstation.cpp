#include "workstation.h"
#include "mainwindow.h"
#include "tcpsocket.h"
#include "udpsocket.h"
#include "ui_mainwindow.h"

#define FILE_LIST 1
#define FILE_TRANSFER 2
#define VOICE_CHAT 3

Workstation::Workstation(MainWindow *mainWindow) {
    int err = 0;
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2,2);

    if ((err = WSAStartup(wVersionRequested, &wsaData)) < 0) {
        qDebug("Workstation::Workstation(): Missing WINSOCK2 DLL.");
        throw "Workstation::Workstation(): Missing WINSOCK2 DLL.";
    }

    QThread * thread = new QThread();

    // Save a pointer to the window
    mainWindowPointer_ = mainWindow;

    // Create TCP socket to listen for requests
    tcpSocket_ = new TCPSocket(mainWindow->winId());

    // Connect signal and slot for WSA events
    connect(mainWindow, SIGNAL(signalWMWSASyncTCPRx(PMSG)),
            tcpSocket_, SLOT(slotProcessWSAEvent(PMSG)));

    // Connect signal and slot for processing a new connection
    connect(tcpSocket_, SIGNAL(signalClientConnected(TCPSocket*)),
            this, SLOT(processConnection(TCPSocket*)));
    connect(tcpSocket_, SIGNAL(signalDataReceived(TCPSocket*)),
            this, SLOT(decodeControlMessage(TCPSocket*)));

    udpSocket_ = new UDPSocket(mainWindow->winId());
    connect(mainWindow, SIGNAL(signalWMWSASyncUDPRx(PMSG)),
            udpSocket_, SLOT(slotProcessWSAEvent(PMSG)));

    tcpSocket_->listen(7000);
    tcpSocket_->moveToThread(thread);

    thread->start();
    //requestFileList();
}

Workstation::~Workstation() {
    delete tcpSocket_;
    delete udpSocket_;
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

/*
-- FUNCTION: connectToServer
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: void Workstation::connectToServer();
--
-- RETURNS: void
--
-- NOTES:
--
*/
void Workstation::connectToServer()
{

}

void Workstation::requestFile()
{

}

/*
-- FUNCTION: connectToServer
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: void Workstation::connectToServer();
--
-- RETURNS: void
--
-- NOTES:
-- This function is triggered when the user wishes to obtain a remote file list.
-- The function establishes a connection to the remote host and sends its file
-- list.
*/
void Workstation::requestFileList()
{
    qDebug("Workstation::requestFileList(); Requesting file list.");

    // Hard coded values pending Joel's implementation of a connect window.
    short port = 7000;
    QString ip("192.168.0.96");
    // Hard coded file list, waiting for Joel
    QStringList *fileNames = new QStringList();
    fileNames->append("song one");
    fileNames->append("song two");
    // End of hard coded values

    // Create the socket
    TCPSocket *requestSocket = new TCPSocket(mainWindowPointer_->winId());

    // Connect to a remote host
    if (!requestSocket->connectRemote(ip, port)) {
      qDebug("Workstation::requestFileList(); Failed to connect to remote.");
      return;
    }

    qDebug("Workstation::requestFileList(); Assuming connection suceeded!.");
    // Send our file list to the remote host
    //requestSocket->send();

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
-- The function gets called when a new connection is established. It connects
-- the socket's receive signal to the workstation's decode message function.
*/
void Workstation::processConnection(TCPSocket* socket)
{
    qDebug("Workstation::processConnection(); processing connection.");

    // Connect the socket to the window's message loop.
    if (!connect(mainWindowPointer_, SIGNAL(signalWMWSASyncTCPRx(int, int)),
                 socket, SLOT(slotProcessWSAEvent(int, int)))) {
        qDebug("Workstation::processConnection(); connect(signalWMWSASyncTCPRx()) failed.");
    }
}

void Workstation::decodeControlMessage(TCPSocket *socket)
{
    qDebug("Workstation::decodeControlMessage(); Decoding...");
    // Disconnect from decode control message
    //disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
    //           this, SLOT(decodeControlMessage(TCPSocket*)));

    QByteArray data = socket->readAll();
    qDebug() << "Workstation::decodeControlMessage(); DataRx: " << data;

    // Check for type of message
    /*
    switch (*buffer[0])
    {
    case FILE_LIST:
        //connect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
        //        this, SLOT(receiveFileList(TCPSocket*)));
        break;
    case FILE_TRANSFER:
        //connect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
        //        this, SLOT(receiveFile(TCPSocket*)));
        break;
    case VOICE_CHAT:
        // Connect to voice chat here
        break;
    default:
        // Since the message is not recognized, close the connection
        //socket->close();
        break;
    }
    */

}

void Workstation::receiveUDP()
{

}

void Workstation::receiveFile(TCPSocket*)
{

}

void Workstation::receiveFileList(TCPSocket *socket)
{
/*
    // Create the local storage for the packet
    QStringList *fileList = new QStringList();
    QDataStream *stream = new QDataStream(*packet);

    // Convert QByteArray to QStringList
    *stream >> *fileList;

    // Signal slot in workstation
    emit signalFileListUpdate(&(*fileList));

    // If this is the last packet, send our own file list
    // Need a way to figure out if we are at the last packet...
    delete fileList;
    *fileList = mainWindowPointer_->getLocalFileList();
    QByteArray *sendBuffer = new QByteArray();
    stream = new QDataStream(*sendBuffer);
    *stream << *fileList;
    //socket->send();
*/

}

QByteArray Workstation::dataStreamFileList()
{
    QStringList fileList = mainWindowPointer_->getLocalFileList();
    QByteArray *returnValue = new QByteArray();
    return *returnValue;
    /*QDataStream *stream = new QDataStream(fileList);

    *returnValue << *stream;
/*
    QByteArray *byteArray = new QByteArray();
    QDataStream *s = new QDataStream(mainWindowPointer_->getLocalFileList());
    *s >> *byteArray;
    return *byteArray;*/
}
