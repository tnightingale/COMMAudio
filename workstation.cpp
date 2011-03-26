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

    // Create UDP socket for voice chat and multicast
    udpSocket_ = new UDPSocket(mainWindow->winId());
    connect(mainWindow, SIGNAL(signalWMWSASyncUDPRx(PMSG)),
            udpSocket_, SLOT(slotProcessWSAEvent(PMSG)));

    // Listen on the TCP socket for other client connections
    tcpSocket_->listen(7000);
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
-- FUNCTION: requestFileList
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: void Workstation::requestFileList();
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
    // Connect the socket's receive signal to the decode message function
    connect(socket, SIGNAL(signalDataReceived(TCPSocket*,QByteArray*)),
            this, SLOT(decodeControlMessage(TCPSocket*,QByteArray*)));
}

void Workstation::decodeControlMessage(TCPSocket *socket, QIODevice *buffer)
{
    // Disconnect from decode control message
    disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*,QByteArray*)),
               this, SLOT(decodeControlMessage(TCPSocket*,QByteArray*)));

    // Read packet from the socket
    QDataStream packetStream(buffer);
    QByteArray packet;
    packetStream >> packet;

    // Put the packet minus the first byte back into the buffer
    buffer << packet.right(packet.length() - 1);

    // Check for type of message
    switch (packet[0])
    {
    case FILE_LIST:
        // Connect the signal in case we receive more data
        connect(socket, SIGNAL(signalDataReceived(TCPSocket*,QByteArray*)),
                this, SLOT(receiveFileListController(TCPSocket*, QByteArray*)));
        // Call function now to deal with rest of packet
        receiveFileListController(&(*socket), &(*buffer));
        break;
    case FILE_TRANSFER:
        // Insert rest of received packet into the current transfers map
        currentTransfers.insert(socket, packet.right(packet.length() - 1));
        // Connect the signal for the type of transfer
        connect(socket, SIGNAL(signalDataReceived(TCPSocket*,QByteArray*)),
                this, SLOT(receiveFile(TCPSocket*, QByteArray*)));
        break;
    case VOICE_CHAT:
        // Connect to voice chat here
        break;
    default:
        // Since the message is not recognized, close the connection
        //socket->close();
        break;
    }

}

void Workstation::receiveUDP()
{

}

void Workstation::receiveFile(TCPSocket*, QIODevice*)
{

}

bool Workstation::processReceivingFile()
{

}

bool Workstation::processReceivingFileList(TCPSocket *socket, QByteArray *packet)
{
    bool isFileListTransferComplete = false;
    // Check to see if this is the last packet
    if (*packet[packet->length() - 1] == '\n')
    {
        // Get rid of the newline character
        packet->truncate(packet->length() - 1);

        QDataStream *stream;
        QStringList fileList;

        // Check to see if we already have already received data
        if (currentTransfers.contains(socket))
        {
            // Get the existing QByteArray out and load it into the stream
            stream = new QDataStream(currentTransfers.value(socket));

            // Append the last packet to the stream
            *stream << *packet;
        }
        else
        {
            // Load the packet into the stream
            stream = new QDataStream(*packet);
        }

        // Stream the packet back into a QStringList
        *stream >> fileList;

        // Send the file list to the main window for procesing
        //mainWindowPointer_->appendToRemote(fileList, socket->getIp());

        // Since processing of the transfer is complete, return true
        isFileListTransferComplete = true;
    }
    else
    {
        QByteArray *buffer;
        // Check to see if this is the first packet
        if (currentTransfers.contains(socket))
        {
            // Since a buffer already exists, add to it
            *buffer = currentTransfers.value(socket);
            buffer->append(*packet);
        }
        else
        {
            // Assign the packet to the buffer
            *buffer = *packet;
        }

        // Insert the buffer into the map
        currentTransfers.insert(socket, *buffer);

        // Since the transfer is not yet complete, return false
        isFileListTransferComplete = false;
    }
    return isFileListTransferComplete;
}

void Workstation::receiveFileListController(TCPSocket *socket, QIODevice *buffer)
{
    // Turn the buffer into a QByteArray for internal processing
    QDataStream packetStream(buffer);
    QByteArray packet;
    packetStream >> packet;

    // If processing is finished
    if(processReceivingFileList(&(*socket), &packet))
    {
        // Disconnect this slot from the received packet signal
        disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*,QByteArray*)),
                   this, SLOT(receiveFileList(TCPSocket*,QByteArray*)));

        // Send our own file list to the other client
        QStringList fileList = mainWindowPointer_->getLocalFileList();
        QByteArray byteArray;
        QDataStream *stream = new QDataStream(byteArray);
        *stream << fileList;
        //socket->write(*stream);
    }
}
