#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QThread>
#include <windowsx.h>
#include <winsock2.h>
#include <QTextStream>
#include <QDataStream>
#include <QFile>
#include <QDebug>

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

class Socket : public QObject
{
    Q_OBJECT
protected:
    /**
     *
     */
    SOCKET socket_;

    /**
     *
     */
    QDataStream * data_;

    QByteArray * socketBuffer_;

    /**
     *
     */
    size_t packetSize_;

    /**
     *
     */
    HWND hWnd_;

public:
    Socket(HWND hWnd, int addressFamily, int connectionType, int protocol);
    Socket(SOCKET socket, HWND hWnd);

    virtual ~Socket() {
        qDebug("Socket::~Socket()");
        closesocket(socket_);

        delete data_;
    }

    /**
     *
     * @author Tom Nightingale.
     */
    SOCKET getSocket() { return socket_; }

    /**
     *
     * @param dataSource
     *
     * @author Tom Nightingale.
     */
    void setDataStream(QByteArray * qba) {
        data_ = new QDataStream(qba, QIODevice::ReadOnly);
    }

    /**
     *
     * @author Tom Nightingale.
     */
    void setDataStream(QFile * file) {
        data_ = new QDataStream(file);
    }

    /**
     *
     * @author Tom Nightingale.
     */
    QDataStream * getDataStream() {
        return data_;
    }

    /**
     *
     * @param packetSize
     *
     * @author Tom Nightingale
     */
    void setPacketSize(size_t packetSize) {
        packetSize_ = packetSize;
    }

    /**
     *
     * @author Tom Nightingale.
     */
    size_t getPacketSize() { return packetSize_; }

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
    void status(QString);

public slots:
    /**
     *
     * @param pMsg
     *
     * @return
     *
     * @author Tom Nightingale.
     */
    virtual bool slotProcessWSAEvent(PMSG pMsg);

};

#endif // SOCKET_H
