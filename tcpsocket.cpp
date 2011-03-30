#include "tcpsocket.h"

TCPSocket::TCPSocket(HWND hWnd)
: Socket(hWnd, AF_INET, SOCK_STREAM, IPPROTO_TCP) {}

TCPSocket::TCPSocket(SOCKET socket, HWND hWnd)
: Socket(socket, hWnd) {}

bool TCPSocket::open(OpenMode mode) {
    int err = 0;
    int flags = FD_CLOSE;

    switch (mode) {
        case QIODevice::ReadOnly:
            flags |= FD_READ;
            break;

        case QIODevice::WriteOnly:
            flags |= FD_CONNECT | FD_WRITE;
            break;

        case QIODevice::ReadWrite:
             flags |= FD_CONNECT | FD_READ | FD_WRITE | FD_ACCEPT;
            break;

        case QIODevice::NotOpen:
             flags = 0;
            break;

        case QIODevice::Append:
        case QIODevice::Truncate:
        case QIODevice::Text:
        case QIODevice::Unbuffered:
        default:
            return false;
            break;
    }

    if ((err = WSAAsyncSelect(socket_, hWnd_, WM_WSAASYNC_TCP, flags))
                              == SOCKET_ERROR) {
        qDebug("TCPSocket::open(): Error setting up async select.");
        return false;
    }

    QIODevice::connect(this, SIGNAL(readyWrite(qint64)),
                       this, SLOT(slotWriteData(qint64)));

    return QIODevice::open(mode);
}

void TCPSocket::slotWriteData(qint64 bytesToWrite) {
    if (loadBuffer(bytesToWrite) < 0) {
        return;
    }
    MSG msg;
    msg.wParam = socket_;
    msg.lParam = 0;
    PMSG pMsg = &msg;
    send(pMsg);
}

void TCPSocket::accept(PMSG pMsg) {
    SOCKET newSocket;
    SOCKADDR_IN client;
    int client_length = sizeof(SOCKADDR_IN);

    if ((newSocket = ::accept(pMsg->wParam, (PSOCKADDR) &client,
                                 &client_length)) == INVALID_SOCKET) {
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            qDebug("TCPSocket:accept(); Error: %d", WSAGetLastError());
            return;
        }
    }

    TCPSocket * clientSocket = new TCPSocket(newSocket, hWnd_);
    QObject::connect(clientSocket, SIGNAL(signalDataReceived(TCPSocket*)),
                     this, SIGNAL(signalDataReceived(TCPSocket*)));
    connectedIp_ = QString(inet_ntoa(client.sin_addr));
    connectedPort_ = client.sin_port;
    emit signalClientConnected(clientSocket);
}

void TCPSocket::send(PMSG pMsg) {
    int err = 0;
    int result = 0;
    DWORD numSent = 0;
    int num = 0;
    size_t bytesToRead = PACKETSIZE;

    WSABUF winsockBuff;

    if (nextTxBuff_ == NULL && loadBuffer(bytesToRead) == 0) {
        qDebug("TCPSocket::send(); Nothing to send.");
        return;
    }

    winsockBuff.buf = nextTxBuff_->data();
    winsockBuff.len = nextTxBuff_->size();

    // TODO: I think i should actually be checking something here, like the
    //       outputBuffer_ status or something.
    while (TRUE) {
        result = WSASend(pMsg->wParam, &winsockBuff, 1, &numSent, 0,
                         NULL, NULL);
        if ((err = WSAGetLastError()) > 0 && err != ERROR_IO_PENDING) {
            qDebug("TCPSocket::send(); Error: %d", err);
            return;
        }
        if (result == WSAEWOULDBLOCK) {
            qDebug("TCPSocket::send(); Socket buffer full: WSAEWOULDBLOCK");
            return;
        }

        delete nextTxBuff_;
        nextTxBuff_ = NULL;
        if ((num = loadBuffer(bytesToRead)) <= 0) {
            qDebug("TCPSocket::send(); Finishing...");
            break;
        }
        winsockBuff.len = num;
    }

    //if (data_->status() == QDataStream::Ok) {
    //    ::shutdown(socket_, SD_SEND);
    //}
}

void TCPSocket::receive(PMSG pMsg) {
    int err = 0;
    DWORD flags = 0;
    DWORD numReceived = 0;
    int bytesWritten = 0;
    WSABUF winsockBuff;

    winsockBuff.len = MAXUDPDGRAMSIZE;
    winsockBuff.buf = (char *) calloc(winsockBuff.len, sizeof(char));

    if (WSARecv(pMsg->wParam, &(winsockBuff), 1, &numReceived, &flags,
                NULL, NULL) == SOCKET_ERROR) {
        if ((err = WSAGetLastError()) != WSA_IO_PENDING) {
            qDebug("TCPSocket::receive(): WSARecv() failed with error %d",
                   err);
            return;
        }
    }

    //qDebug() << "TCPSocket::receive(); DataRx: " << winsockBuff.buf;

    while (numReceived > 0) {
        // Lock mutex here.
        inputBuffer_->open(QBuffer::WriteOnly);
        bytesWritten = inputBuffer_->write(winsockBuff.buf, numReceived);
        numReceived -= bytesWritten;
        inputBuffer_->close();
        // Unlock mutex.

        if (numReceived == 0) {
            break;
        }
    }

    delete[] winsockBuff.buf;

    emit readyRead();
    emit signalDataReceived(this);
}

void TCPSocket::connect(PMSG) {
    if (loadBuffer(PACKETSIZE) < 0) {
        qDebug("TCPSocket::connect(); Cannot read from data source!");
        throw "TCPSocket::connect(); Cannot read from data source!";
    }
}

int TCPSocket::loadBuffer(size_t bytesToRead) {
    // Lock mutex here
    if (outputBuffer_->bytesAvailable() == 0) {
        return 0;
    }
    nextTxBuff_ = new QByteArray(bytesToRead, '\0');
    // TODO: I had problems with this, might need to call
    //       QByteArray::readRawData().
    outputBuffer_->open(QIODevice::ReadOnly);
    int bytesRead = outputBuffer_->read(nextTxBuff_->data(), bytesToRead);
    outputBuffer_->close();

    // Removing stuff that was read from the buffer.
    QByteArray buffer = outputBuffer_->buffer();
    buffer.remove(0, bytesRead);
    outputBuffer_->setData(buffer);
    // Unlock mutex.

    nextTxBuff_->resize(bytesRead);
    return bytesRead;
}

bool TCPSocket::listen(int port) {
    int err = 0;
    SOCKADDR_IN sockAddrIn;

    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_port = htons(port);
    sockAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);

    if (!Socket::listen(&sockAddrIn)) {
        return false;
    }

    if ((::listen(socket_, 5)) == SOCKET_ERROR) {
        err = WSAGetLastError();
        qDebug("TCPSocket::listen(); Error: %d.", err);
        return false;
    }

    return true;
}

bool TCPSocket::connectRemote(QString address, int port) {
    PHOSTENT host;
    SOCKADDR_IN serverSockAddrIn;
    int err = 0;

    if ((host = gethostbyname(address.toAscii().data())) == NULL) {
        err = GetLastError();
        qDebug("Client::writeTCP(): Unknown server address. Error: %d.", err);
        return false;
    }

    memcpy((char*) &serverSockAddrIn.sin_addr, host->h_addr, host->h_length);
    serverSockAddrIn.sin_family = AF_INET;
    serverSockAddrIn.sin_port = htons(port);

    if ((err = ::connect(socket_, (PSOCKADDR) &serverSockAddrIn,
                   sizeof(SOCKADDR_IN))) == SOCKET_ERROR) {
        if ((err = WSAGetLastError()) != WSAEWOULDBLOCK) {
            qDebug("TCPSocket::connectRemote(): Connect failed. Error: %d",
                   WSAGetLastError());
            return false;
        }
    }
    connectedIp_ = address;
    connectedPort_ = port;
    return true;
}

//void TCPSocket::slotProcessWSAEvent(PMSG pMsg) {
void TCPSocket::slotProcessWSAEvent(int wParam, int lParam) {
    MSG msg;
    msg.wParam = wParam;
    msg.lParam = lParam;
    PMSG pMsg = &msg;

    if (pMsg->wParam != socket_) {
        return;
    }

    if (WSAGETSELECTERROR(pMsg->lParam)) {
        qDebug("TCPSocket::slotProcessWSAEvent(): %d: Socket failed. Error: %d",
              (int) pMsg->wParam, WSAGETSELECTERROR(pMsg->lParam));
        return;
    }

    switch (WSAGETSELECTEVENT(pMsg->lParam)) {
        case FD_ACCEPT:
            qDebug("TCPSocket::slotProcessWSAEvent: %d: FD_ACCEPT.",
                   (int) pMsg->wParam);
            accept(pMsg);
            break;

        case FD_READ:
            qDebug("TCPSocket::slotProcessWSAEvent: %d: FD_READ.",
                   (int) pMsg->wParam);
            receive(pMsg);
            break;

        case FD_CONNECT:
            qDebug("TCPSocket::slotProcessWSAEvent: %d: FD_CONNECT.",
                   (int) pMsg->wParam);
            connect(pMsg);
            break;

        case FD_WRITE:
            qDebug("TCPSocket::slotProcessWSAEvent: %d: FD_WRITE.",
                   (int) pMsg->wParam);
            send(pMsg);
            break;

        default:
            Socket::slotProcessWSAEvent(pMsg);
            break;
    }

    return;
}

