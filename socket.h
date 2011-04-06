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

/** Defining some base buffer sizes. */
#define MSGSIZE 1024
#define MAXUDPDGRAMSIZE 4096
#define PACKETSIZE 2048

/** Custom Windows messages for use with WSAAsyncSelect(). */
#define WM_WSAASYNC_TCP (WM_USER + 1)
#define WM_WSAASYNC_UDP (WM_USER + 2)

/** Forward-declarations */
class Buffer;

/**
 * This is supposed to be used as an abstract class that encapsulates and 
 * provides the basic functionality of a socket. It should be extended to 
 * add extra socket type/protocol specific funtionality.
 *
 * It provides a wrapper around windows asynchronous sockets using the WSA Win32
 * API call WSAASyncSelect() which utilizes the Windows message loop.
 *
 * @author Tom Nightingale.
 */
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

    /** The ip address of the remote machine this socket is connected to. */
    QString connectedIp_;

    /** The port of the remote machine this socket is connected to. */
    short connectedPort_;

    /** Mutex used to lock critical sections involving the output buffer. */
    QMutex* sendLock_;

    /** Mutex used to lock critical sections involving the input buffer. */
    QMutex* receiveLock_;


    /** 
     * Virtual method implemented from QIODevice.
     *
     * @param data A empty buffer to read to.
     * @param maxSize The max amount of to data to read.
     *
     * @author Tom Nightingale
     */
    virtual qint64 readData(char * data, qint64 maxSize);

    /** 
     * Virtual method implemented from QIODevice.
     *
     * @param data Buffer containing the data to write.
     * @param maxSize The max amount of to data to write.
     *
     * @author Tom Nightingale
     */
    virtual qint64 writeData(const char * data, qint64 maxSize);

    /**
     * Load the next buffer for writing to the socket.
     *
     * @param bytesToRead The number of bytes to load.
     *
     * @author Tom Nightingale
     */
    int loadBuffer(size_t bytesToRead);

    /**
     * Called whenever Windows notifies the socket that there is data available
     * to be read from the socket.
     *
     * NOTE: MUST be overridden by any subclasses.
     *
     * @param pMsg The message containing the Windows notification.
     *
     * @author Tom Nightingale
     */
    virtual void receive(PMSG pMsg) {}

    /**
     * Called whenever Windows notifies the socket that there is data that
     * should be written.
     *
     * NOTE: MUST be overridden by any subclasses.
     *
     * @param pMsg Contains the Windows message.
     *
     * @author Tom Nightingale
     */
    virtual void send(PMSG pMsg) {}

public:
    /**
     * Constructor.
     * Creates the socket and notifies Windows to prepare it for usage.
     * 
     * @param hWnd           A pointer to the applications main window. This is 
     *                       used to hook into the applications winproc.
     * @param addressFamily  Specify the address family this socket will be
     *                       used with.
     * @param connectionType The type of connection this socket is. Stream or
     *                       Datagram.
     * @param protocol       The protocol to be used with this socket.
     *
     * @author Tom Nightingale
     */
    Socket(HWND hWnd, int addressFamily, int connectionType, int protocol);

    /**
     * Overloaded copy constructor.
     * Creates a socket using an existing socket.
     *
     * @param socket The existing initialized socket handle.
     * @param hWnd A pointer to the main window.
     * @param remoteAddr A remote address this socket will be connected to.
     *
     * @author Tom Nightingale.
     */
    Socket(SOCKET socket, HWND hWnd, QString remoteAddr);

    virtual ~Socket() {
        qDebug("Socket::~Socket() %d", (int) socket_);
        closesocket(socket_);
    }

    /**
     * Returns the socket.
     *
     * @author Tom Nightingale.
     */
    SOCKET getSocket() { return socket_; }

    /**
     * Sets the socket to listen for incoming connections.
     *
     * @param pSockAddr The address to listen to.
     *
     * @author Tom Nightingale.
     */
    virtual bool listen(PSOCKADDR_IN pSockAddr);

    /**
     * Closes the socket.
     *
     * @param pMsg The Windows message notifying the socket of its closure.
     *
     * @author Tom Nightingale.
     */
    void close(PMSG pMsg);

    /**
     * Returns the connected IP address of the socket.
     *
     * @author Luke Queenan
     */
    QString getIp() { return connectedIp_; }

    /**
     * Returns the connected port of the socket.
     *
     * @author Luke Queenan
     */
    short getPort() { return connectedPort_; }

    /**
     * Manually close an open connection to this socket.
     *
     * @author Tom Nightingale.
     */
    void closeConnection();

    /**
     * Overridden function from QIODevice.
     * Returns true, this is a sequential IO device.
     *
     * @author Tom Nightingale.
     */
    bool isSequential() const;

    /**
     * Overridden function from QIODevice.
     * Returns the size in bytes of this IO device.
     *
     * @author Tom Nightingale.
     */
    qint64 size() const;

    /**
     * Overridden function from QIODevice.
     * Returns the number of bytes available for reading on this IO device.
     *
     * @author Tom Nightingale.
     */
    qint64 bytesAvailable() const;

signals:
    /** 
     * Emitted when the socket is closed.
     *
     * @author Tom Nightingale
     */
    void signalSocketClosed();

    /** 
     * Emitted when the socket has data in its buffer ready for writing to its
     * socket.
     *
     * @param The number of bytes avaiable for writing.
     *
     * @author Tom Nightingale
     */
    void readyWrite(qint64);
    
    /**
     * Returns the IO devices status.
     *
     * @param The status.
     *
     * @author Tom Nightingale
     */
    void status(QString);

    /**
     * Emitted when data is received on this socket and therefore available for
     * reading.
     *
     * @param socket This socket.
     *
     * @author Tom Nightingale
     */
    void signalDataReceived(Socket* socket);

public slots:
    /**
     * Used to process Windows asynchronous socket events received by this 
     * socket.
     *
     * @param pMsg The windows message notifying about the event.
     *
     * @author Tom Nightingale.
     */
    virtual void slotProcessWSAEvent(PMSG pMsg);

    /**
     * Write data to the socket.
     *
     * @param bytesToWrite The number of bytes to write to teh socket.
     *
     * @author Tom Nightingale.
     */
    void slotWriteData(qint64 bytesToWrite);
};

#endif // SOCKET_H
