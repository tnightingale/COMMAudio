#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QBuffer>
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
#define MAXUDPDGRAMSIZE 65507
#define PACKETSIZE 4096

class Socket;

typedef struct _DATA_ {
    WSABUF winsockBuff;
    Socket* socket;
    SOCKET clientSD;
} DATA, *PDATA;

typedef struct _STATS_ {
    int totalBytes;
    int totalPackets;
    DWORD startTime;
    DWORD finishTime;
} STATS, *PSTATS;

class Socket : public QIODevice {
  Q_OBJECT
protected:
    /** The socket descriptor for which this object is encapsulating. */
    SOCKET socket_;

    /** Window handle, due to windows idiosyncracies, all sockets are tied to
     *  a window. */
    HWND hWnd_;

    /** The internal output buffer for writing data. */
    QBuffer* outputBuffer_;

    /** The internal input buffer for reading data. */
    QBuffer* inputBuffer_;

    /** This tracks the next block of data to transmit. Needs to be persistent
     *  incase WSASend() returns with WSAEWOULDBLOCK and block needs to be
     *  resent */
    QByteArray* nextTxBuff_;

    /** These are probably going to be passed on to the writeThread. */
    size_t packetSize_;

    QMutex *lock_;

    virtual qint64 readData(char * data, qint64 maxSize);
    virtual qint64 writeData(const char * data, qint64 maxSize);

public:
    Socket(HWND hWnd, int addressFamily, int connectionType, int protocol);
    Socket(SOCKET socket, HWND hWnd);

    virtual ~Socket() {
        qDebug("Socket::~Socket() %d", (int) socket_);
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

signals:
    void signalSocketClosed();
    void readyWrite(qint64);
    void status(QString);

public slots:
    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale.
     */
    virtual void slotProcessWSAEvent(PMSG pMsg);

};

#endif // SOCKET_H
