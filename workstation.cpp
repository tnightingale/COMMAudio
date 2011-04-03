#include "workstation.h"
#include "mainwindow.h"
#include "tcpsocket.h"
#include "udpsocket.h"
#include "ui_mainwindow.h"
#include "filedata.h"

#define FILE_LIST 1
#define FILE_TRANSFER 2
#define VOICE_CHAT 3

Workstation::Workstation(MainWindow* mainWindow)
    : socketThread_(new QThread()), mainWindowPointer_(mainWindow) {
    int err = 0;
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2,2);

    if ((err = WSAStartup(wVersionRequested, &wsaData)) < 0) {
        qDebug("Workstation::Workstation(): Missing WINSOCK2 DLL.");
        throw "Workstation::Workstation(): Missing WINSOCK2 DLL.";
    }

    // Open TCP socket for reading and writing.
    tcpSocket_ = new TCPSocket(mainWindow->winId());
    // Connect signal and slot for WSA events
    connect(mainWindow, SIGNAL(signalWMWSASyncTCPRx(int, int)),
            tcpSocket_, SLOT(slotProcessWSAEvent(int, int)));
    tcpSocket_->open(QIODevice::ReadWrite);

    // Connect signal and slot for processing a new connection
    connect(tcpSocket_, SIGNAL(signalClientConnected(TCPSocket*)),
            this, SLOT(processConnection(TCPSocket*)));
    connect(tcpSocket_, SIGNAL(signalDataReceived(TCPSocket*)),
            this, SLOT(decodeControlMessage(TCPSocket*)));

    // Connect signal and slot for WSA events.
    udpSocket_ = new UDPSocket(mainWindow->winId());
    connect(mainWindow, SIGNAL(signalWMWSASyncUDPRx(int, int)),
            udpSocket_, SLOT(slotProcessWSAEvent(int, int)));

    // Connect the GUI button signals to the functions in here
    connect(mainWindow, SIGNAL(requestPlaylist(QString, short)),
            this, SLOT(requestFileList(QString, short)));
    connect(mainWindow, SIGNAL(requestFile(QString, short, QString)),
            this, SLOT(requestFile(QString, short ,QString)));

    // Listen on the TCP socket for other client connections
    if(!tcpSocket_->listen(7000))
    {
        tcpSocket_->listen(7001);
    }

    tcpSocket_->moveToThread(socketThread_);
    socketThread_->start();

}

Workstation::~Workstation() {
    delete tcpSocket_;
    delete udpSocket_;
    // Should delete the current transfers map here too?
}

void Workstation::sendFile(TCPSocket *socket, QByteArray *data)
{
    // Create file name
    QDataStream fileNameStream(&(*data), QIODevice::ReadOnly);
    QString fileName;
    fileNameStream >> fileName;

    // Open the file
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly)){
        // Should probably do something better here...
        return;
    }

    // Read file into a byte array for sending
    QByteArray packet = file.readAll();

    // Create the control packet
    int length = packet.size();
    QByteArray len((const char *)&length, sizeof(int));
    packet.insert(0, len);

    /* Test for retrieving length
    int test;
    memcpy(&test, len, sizeof(int));
    */

    // Send our file to the other client
    socket->write(packet);
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
-- Function for connecting to a multicast server.
*/
void Workstation::connectToServer()
{

}

void Workstation::requestFile(QString ip, short port, QString songPath)
{
    // Create the socket
    TCPSocket *requestSocket = new TCPSocket(mainWindowPointer_->winId());
    connect(mainWindowPointer_, SIGNAL(signalWMWSASyncTCPRx(int,int)),
            requestSocket, SLOT(slotProcessWSAEvent(int,int)));

    // Connect to a remote host
    if (!requestSocket->connectRemote(ip, port)) {
        qDebug("Workstation::requestFile(); Failed to connect to remote.");
        return;
    }

    requestSocket->open(QIODevice::ReadWrite);
    requestSocket->moveToThread(socketThread_);

    // Convert the file path into a byte array via a stream.
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << songPath;

    // Create the control packet
    byteArray.insert(0, FILE_TRANSFER);
    byteArray.append('\n');

    // Send the file path to the client
    requestSocket->write(byteArray);

    // Put the transfer into the current transfer map
    currentTransfers.insert(requestSocket, new FileData(this, songPath, port));

    // Connect the signal for receiving the file
    connect(requestSocket, SIGNAL(signalDataReceived(TCPSocket*)),
            this, SLOT(receiveFileController(TCPSocket*)));
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
-- list. After sending the file list, it connects the signal and slot for
-- receiving the other clients file list.
*/
void Workstation::requestFileList(QString ip, short port)
{
    // Create the socket
    TCPSocket *requestSocket = new TCPSocket(mainWindowPointer_->winId());
    connect(mainWindowPointer_, SIGNAL(signalWMWSASyncTCPRx(int,int)),
            requestSocket, SLOT(slotProcessWSAEvent(int,int)));

    // Connect to a remote host
    if (!requestSocket->connectRemote(ip, port)) {
        qDebug("Workstation::requestFileList(); Failed to connect to remote.");
        return;
    }

    requestSocket->open(QIODevice::ReadWrite);
    requestSocket->moveToThread(socketThread_);

    // Get our local file list and convert it to a data stream
    QStringList fileList = mainWindowPointer_->getLocalFileList();
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream << fileList.toSet();

    // Create the control packet
    byteArray.insert(0, FILE_LIST);
    byteArray.append('\n');

    // Send our own file list to the other client
    requestSocket->write(byteArray);

    // Put the socket into the current transfers map
    currentTransfers.insert(requestSocket, new FileData(this,port));

    // Connect the signal for receiving the other client's file list
    connect(requestSocket, SIGNAL(signalDataReceived(TCPSocket*)),
            this, SLOT(requestFileListController(TCPSocket*)));
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
    // Connect the socket to the window's message loop.
    if (!connect(mainWindowPointer_, SIGNAL(signalWMWSASyncTCPRx(int, int)),
                 socket, SLOT(slotProcessWSAEvent(int, int)))) {
        qDebug("Workstation::processConnection(); connect(signalWMWSASyncTCPRx()) failed.");
    }
    if (!socket->open(QIODevice::ReadWrite)) {
        qDebug("Workstation::processConnection(); could not open client socket.");
    }
}

/*
-- FUNCTION: decodeControlMessage
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: void Workstation::decodeControlMessage(TCPSocket *socket);
--
-- RETURNS: void
--
-- NOTES:
-- The function gets called when the first packet from a new connection is
-- received. The function will strip the first byte from the packet and call
-- the correct corresponding transfer function to deal with the rest of the
-- transfer.
*/
void Workstation::decodeControlMessage(TCPSocket *socket)
{
    // Disconnect from decode control message
    disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
               tcpSocket_, SIGNAL(signalDataReceived(TCPSocket*)));

    // Read and store the first byte for the control message
    QByteArray messageType = socket->read(1);
    // TODO: For some reason we seem to receive a bunch of '\0' chars before the
    //       SIGNAL(signalDataReceived()) is disconnected. The sanity check
    //       and return; below shouldn't be necessary. It is just a bandaid fix
    //       and doesn't actually solve the problem. However, at the end of the
    //       day, sending extra NULL data is the least of our concerns.
    if (messageType.size() == 0) {
        return;
    }

    // Check for type of message
    switch (messageType.at(0))
    {
    case FILE_LIST:
        // Connect the signal in case we receive more data
        connect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
                this, SLOT(receiveFileListController(TCPSocket*)));
        // Call function now to deal with rest of packet
        currentTransfers.insert(socket, new FileData);
        receiveFileListController(&(*socket));
        break;
    case FILE_TRANSFER:
        // Connect the signal in case we receive more data
        connect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
                this, SLOT(sendFileController(TCPSocket*)));
        currentTransfers.insert(socket, new FileData);
        sendFileController(&(*socket));
        break;
    case VOICE_CHAT:
        // Connect to voice chat here
        break;
    default:
        // Since the message is not recognized, close the connection
        //socket->deleteLater();
        break;
    }
}

void Workstation::receiveUDP()
{

}

void Workstation::sendFileController(TCPSocket *socket)
{
    // Read the packet from the socket
    QByteArray packet = socket->readAll();

    // If processing is finished
    if (processReceivingFileRequest(&(*socket), &packet))
    {
        // Disconnect this slot from the received packet signal
        disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
                   this, SLOT(sendFileController(TCPSocket*)));
    }
}

bool Workstation::processReceivingFileRequest(TCPSocket *socket, QByteArray *packet)
{
    bool isReceivingFileRequestFinished = false;

    if (packet->at(packet->length() - 1) == '\n')
    {
        // Get rid of the newline character
        packet->truncate(packet->length() - 1);

        FileData *fileData = currentTransfers.value(socket);

        // Append any new data to any existing data
        fileData->append(*packet);

        // Send the file
        sendFile(socket, &(fileData->getData()));

        // Remove the file transfer
        // Not sure if we can do this right after sending the data, this might
        // destroy the socket?
        currentTransfers.remove(socket);

        // Since processing of the transfer is complete, return true
        isReceivingFileRequestFinished = true;
    }
    else
    {
        // Append newly received data
        FileData* fileData = currentTransfers.value(socket);
        fileData->append(*packet);

        // Since the transfer is not yet complete, return false
        isReceivingFileRequestFinished = false;
    }

    return isReceivingFileRequestFinished;
}

void Workstation::receiveFileController(TCPSocket *socket)
{
    // TODO: PLEASE READ ME:
    // Because we use readAll() to read data from the socket, it is quite
    // possible for us to read two or more packets worth of data from the
    // socket. This causes a problem as this method gets called once for
    // EVERY packet received. This leads to the possibility that we have
    // already read the entire file from the socket and quite likely deleted
    // the socket already, leaving the *socket parameter passed into this
    // method a segfaulting monster! Fortunately we track our sockets in the
    // currentTransfers map and can very easily validate the *socket.
    if (!currentTransfers.contains(socket)) {
        return;
    }

    // Read the packet from the socket
    QByteArray packet = socket->readAll();

    // TODO: PLEASE READ ME:
    // This is ensuring that we haven't read a bunch of NULLs off the socket.
    // (This does actually seem to occur sometimes, please see the beginning of
    // decodeControlMessage() for details.
    if (packet.size() == 0) {
        return;
    }

    // If processing is finished
    if(processReceivingFile(&(*socket), &packet))
    {
        // Disconnect this slot from the received packet signal
        if (!disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
                   this, SLOT(receiveFileController(TCPSocket*)))) {
        }

        // Close the socket
        socket->deleteLater();
    }
}

bool Workstation::processReceivingFile(TCPSocket* socket, QByteArray* packet)
{
    bool isFileListTransferComplete = false;

    // Check to see if this is the last packet
    // TODO: DANGER!!! WE CANNOT ASSUME THAT AN AUDIO FILE WILL *NOT* CONTAIN
    //       '\n's!!!!! NEED TO COME UP WITH AN ALTERNATIVE APPROACH.

    // Get the file data object for this transfer
    FileData *fileData = currentTransfers.value(socket);

    // Check to see if we have the size of the packet yet (first packet)
    if (fileData->getTotalSize() == 0)
    {
        // Get the first eight bytes, convert them to a long long, remove them
        // from the packet and set the length in the file data
        QByteArray length = packet->left(8);
        int packetLength;
        memcpy(&packetLength, length, sizeof(int));
        *packet = packet->right(packet->size() - 8);
        fileData->setTotalSize(packetLength);
    }

    // Append any new data to any existing data
    fileData->append(*packet);

    // Check to see if we have received all the data
    if (fileData->transferComplete())
    {
        // Write all the data to disk
        fileData->writeToFile();

        // Remove the file transfer
        currentTransfers.remove(socket);

        isFileListTransferComplete = true;
    }

    return isFileListTransferComplete;
}

/*
-- FUNCTION: processReceivingFileList
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: bool Workstation::processReceivingFileList(TCPSocket *socket,
--            QByteArray *packet);
--
-- RETURNS: true if the transfer is complete, false if more data is expected
--
-- NOTES:
-- This function receives a file list packet and assembles the entire list. Once
-- the list has been received the function returns true, or if there is more
-- data expected, false.
*/
bool Workstation::processReceivingFileList(TCPSocket *socket, QByteArray *packet)
{
    bool isFileListTransferComplete = false;
    // Check to see if this is the last packet
    if (packet->at(packet->length() - 1) == '\n')
    {
        // Get rid of the newline character
        packet->truncate(packet->length() - 1);

        QStringList fileList;

        FileData *fileData = currentTransfers.value(socket);
        // Append any new data to any existing data
        fileData->append(*packet);

        // Get all the stored data from the current transfer
        QByteArray data = fileData->getData();

        // Load the data into the stream
        QDataStream stream(&data, QIODevice::ReadOnly);

        // Stream the packet back into a QStringList
        stream >> fileList;

        // Send the file list to the main window for procesing
        mainWindowPointer_->appendToRemote(fileList, socket->getIp(), fileData->getPort());

        // Remove the file transfer
        currentTransfers.remove(socket);

        // Since processing of the transfer is complete, return true
        isFileListTransferComplete = true;
    }
    else
    {
        // Append newly received data
        FileData *fileData = currentTransfers.value(socket);
        fileData->append(*packet);

        // Since the transfer is not yet complete, return false
        isFileListTransferComplete = false;
    }
    return isFileListTransferComplete;
}

/*
-- FUNCTION: receiveFileListController
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: void Workstation::receiveFileListController(TCPSocket *socket);
--
-- RETURNS: void
--
-- NOTES:
-- This function is the controller function for the server side file list
-- transfer. It receives a file list and then sends its own.
*/
void Workstation::receiveFileListController(TCPSocket *socket)
{
    // Read the packet from the socket
    QByteArray packet = socket->readAll();

    // If processing is finished
    if(processReceivingFileList(&(*socket), &packet))
    {
        // Disconnect this slot from the received packet signal
        disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
                   this, SLOT(receiveFileListController(TCPSocket*)));

        // Send our own file list to the other client
        QStringList fileList = mainWindowPointer_->getLocalFileList();
        QByteArray byteArray;
        QDataStream stream(&byteArray, QIODevice::WriteOnly);
        stream << fileList.toSet();

        // Create the control packet
        byteArray.append('\n');

        // Send our own file list to the other client
        socket->write(byteArray);
    }
}

/*
-- FUNCTION: requestFileListController
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- INTERFACE: void Workstation::requestFileListController(TCPSocket *socket);
--
-- RETURNS: void
--
-- NOTES:
-- This function is the controller function for the client side file list
-- transfer. It receives a file list and then disconnects.
*/
void Workstation::requestFileListController(TCPSocket *socket)
{
    // TODO: There is a possiblility that for REALLY long filelists socket will
    //       have been deleted. This should be investigated further.
    //       See receiveFileController() for details.)
    // Read the packet from the socket
    QByteArray packet = socket->readAll();

    // If processing is finished
    if(processReceivingFileList(&(*socket), &packet))
    {
        // Disconnect this slot from the received packet signal
        disconnect(socket, SIGNAL(signalDataReceived(TCPSocket*)),
                   this, SLOT(requestFileListController(TCPSocket*)));

        // Close the socket
        delete socket;
    }
}
