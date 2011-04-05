#include "socket.h"
#include "buffer.h"

Socket::Socket(HWND hWnd, int addressFamily, int connectionType, int protocol)
: hWnd_(hWnd), outputBuffer_(new Buffer()), inputBuffer_(new Buffer()),
  nextTxBuff_(NULL), sendLock_(new QMutex()), receiveLock_(new QMutex()) {

    if ((socket_ = WSASocket(addressFamily, connectionType, protocol, NULL, 0,
                             WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        int err = WSAGetLastError();
        qDebug("Socket::Socket(): Can't create socket. (%d)", err);
        throw "Socket::Socket(); Can't create socket.";
    }

    connect(this, SIGNAL(signalSocketClosed()),
            this, SLOT(deleteLater()));
}

Socket::Socket(SOCKET socket, HWND hWnd, QString remoteAddr)
: socket_(socket), hWnd_(hWnd), outputBuffer_(new Buffer()),
  inputBuffer_(new Buffer()), nextTxBuff_(NULL), connectedIp_(remoteAddr),
  sendLock_(new QMutex()), receiveLock_(new QMutex()) {

    connect(this, SIGNAL(signalSocketClosed()),
            this, SLOT(deleteLater()));
}

qint64 Socket::readData(char * data, qint64 maxSize) {
    qint64 bytesRead = 0;
    //qDebug("Socket::readData(); Socket: %d", (int) socket_);

    // Mutex lock here.
    QMutexLocker locker(receiveLock_);
    QByteArray readData = inputBuffer_->read(maxSize);
    bytesRead = readData.size();
    memcpy(data, readData.data(), bytesRead);
    // Mutex unlock.

    return bytesRead;
}

qint64 Socket::writeData(const char * data, qint64 maxSize) {
    qint64 bytesWritten = 0;

    // Mutex lock here.
    QMutexLocker locker(sendLock_);
    QByteArray writeData(data, maxSize);
    outputBuffer_->write(writeData);
    bytesWritten = maxSize;
    locker.unlock();
    // Mutex unlock.

    emit readyWrite(bytesWritten);
    return bytesWritten;
}

void Socket::slotWriteData(qint64 bytesToWrite) {
    if (loadBuffer(bytesToWrite) < 0) {
        return;
    }

    MSG msg;
    msg.wParam = socket_;
    msg.lParam = 0;
    PMSG pMsg = &msg;
    send(pMsg);
}

int Socket::loadBuffer(size_t bytesToRead) {
    // Lock mutex here
    QMutexLocker locker(sendLock_);
    if (outputBuffer_->size() == 0) {
        return 0;
    }

    nextTxBuff_ = new QByteArray(outputBuffer_->read(bytesToRead));
    int bytesRead = nextTxBuff_->size();
    locker.unlock();
    // Unlock mutex.

    return bytesRead;
}


qint64 Socket::size() const {
    return bytesAvailable();
}

qint64 Socket::bytesAvailable() const {
    QMutexLocker locker(receiveLock_);
    qDebug("Socket::bytesAvailable(); Bytes: %d", inputBuffer_->size() + QIODevice::bytesAvailable());
    qint64 size = inputBuffer_->size() + QIODevice::bytesAvailable();

    return size;
}

bool Socket::listen(PSOCKADDR_IN pSockAddr) {
    int err = 0;

    if ((err = bind(socket_, (PSOCKADDR) pSockAddr, sizeof(SOCKADDR))
        == SOCKET_ERROR)) {
        qDebug("Socket::listen(): Can't bind to socket. Error: %d",
               WSAGetLastError());
        return false;
    }

    qDebug("Socket::listen(); listening to port %d", ntohs(pSockAddr->sin_port));

    return true;
}

void Socket::close(PMSG pMsg) {
    shutdown(pMsg->wParam, SD_SEND);
    QIODevice::close();
    emit signalSocketClosed();
}

void Socket::closeConnection() {
    ::shutdown(socket_, SD_BOTH);
}


void Socket::slotProcessWSAEvent(PMSG pMsg) {
    if (WSAGETSELECTERROR(pMsg->lParam)) {
        //qDebug("Socket::slotProcessWSAEvent(): %d: Socket failed with error %d",
              //(int) pMsg->wParam, WSAGETSELECTERROR(pMsg->lParam));
        return;
    }

    if (pMsg->wParam != socket_) {
        return;
    }

    switch (WSAGETSELECTEVENT(pMsg->lParam)) {

        case FD_CLOSE:
            //qDebug("Socket::slotProcessWSAEvent(); %d: FD_CLOSE.",
                   //(int) pMsg->wParam);
            close(pMsg);
            break;
    }

    return;
}

bool Socket::isSequential() const
{
    return true;
}
