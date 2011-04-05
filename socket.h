#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <windowsx.h>
#include <winsock2.h>
#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <QDebug>
#include <QMutex>
#include <QMutexLocker>

#define MSGSIZE 1024
#define WM_WSAASYNC_TCP (WM_USER + 1)
#define WM_WSAASYNC_UDP (WM_USER + 2)
#define DATABUFSIZE 2097152
#define MAXUDPDGRAMSIZE 4096
#define PACKETSIZE 2048

class Buffer;

class Socket : public QIODevice {
  Q_OBJECT
protected:
    /** The socket descriptor for which this object is encapsulating. */
    SOCKET socket_;

    /** Window handle, due to windows idiosyncracies, all sockets are tied to
     *  a window. */
    HWND hWnd_;

    /** The internal output buffer for writing data. */
    Buffer* outputBuffer_;

    /** The internal input buffer for reading data. */
    Buffer* inputBuffer_;

    /** This tracks the next block of data to transmit. Needs to be persistent
     *  incase WSASend() returns with WSAEWOULDBLOCK and block needs to be
     *  resent */
    QByteArray* nextTxBuff_;

    /** These are probably going to be passed on to the writeThread. */
    size_t packetSize_;

    QString connectedIp_;
    short connectedPort_;

    QMutex* sendLock_;
    QMutex* receiveLock_;

    virtual qint64 readData(char * data, qint64 maxSize);
    virtual qint64 writeData(const char * data, qint64 maxSize);

    /**
     *
     * @param bytesToRead
     *
     * @author Tom Nightingale
     */
    int loadBuffer(size_t bytesToRead);

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    virtual void receive(PMSG pMsg) {}

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    virtual void send(PMSG pMsg) {}

public:
    Socket(HWND hWnd, int addressFamily, int connectionType, int protocol);
    Socket(SOCKET socket, HWND hWnd, QString remoteAddr);

    virtual ~Socket() {
        qDebug("Socket::~Socket() %d", (int) socket_);
        emit signalSocketClosed();
        closesocket(socket_);
    }

    /**
     *
     * @author Tom Nightingale.
     */
    SOCKET getSocket() { return socket_; }

    /**
     *
     * @param pSockAddr
     *
     * @return
     *
     * @author Tom Nightingale.
     */
    virtual bool listen(PSOCKADDR_IN pSockAddr);

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale.
     */
    void close(PMSG pMsg);

    /**
     * Returns the connected IP address of the socket.
     *
     * @author Luke Queenan
     */
    QString getIp()
    {
        return connectedIp_;
    }

    /**
     * Returns the connected port of the socket.
     *
     * @author Luke Queenan
     */
    short getPort()
    {
        return connectedPort_;
    }


    bool isSequential() const;
    qint64 size() const;
    qint64 bytesAvailable() const;

signals:
    void signalSocketClosed();
    void readyWrite(qint64);
    void status(QString);

    /**
     *
     * @param socket
     * @param buffer
     *
     * @author Tom Nightingale
     */
    void signalDataReceived(Socket* socket);

public slots:
    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale.
     */
    virtual void slotProcessWSAEvent(PMSG pMsg);

    /**
     *
     */
    void slotWriteData(qint64 bytesToWrite);

};

#endif // SOCKET_H
