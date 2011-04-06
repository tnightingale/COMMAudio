#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include "socket.h"

/**
 * Wrapper class for a Windows asynchronous TCP socket. Base socket
 * functionality is provided by parent class Socket.
 *
 * @author Tom Nightingale
 */
class TCPSocket : public Socket
{
    Q_OBJECT

protected:
    /**
     * Manages reading from the socket into the internal socket buffer.
     *
     * @param pMsg The Windows message notifying the socket that there is data
     *             waiting to be read.
     *
     * @author Tom Nightingale
     */
    virtual void receive(PMSG pMsg);

    /**
     * Manages writing data to the socket to send a connected remote machine.
     *
     * @param pMsg The Windows message notifying of this event.
     *
     * @author Tom Nightingale
     */
    virtual void send(PMSG pMsg);

public:
    /**
     * Constructor.
     *
     * @param hWnd The handle to the applications mainwindow, used as a link to
     *             the underlying winproc.
     *
     * @author Tom Nightingale
     */
    TCPSocket(HWND hWnd);

    /**
     * Constructor.
     *
     * @param hWnd       The handle to the applications mainwindow, used as a 
     *                   link to the underlying winproc.
     * @param remoteAddr The remote address to connect this socket to.
     *
     * @author Tom Nightingale
     */
    TCPSocket(SOCKET socket, HWND hWnd, QString remoteAddr);

    /**
     * Reimplemented from QIODevice.
     * Sets this IO device's open mode to the mode specified.
     * 
     * @param mode The mode to set this device to.
     *
     * @author Tom Nightingale
     */
    virtual bool open(OpenMode mode);

    /**
     * Accepts a connection from a client socket.
     *
     * @param pMsg The Windows message notifying of this event.
     *
     * @author Tom Nightingale
     */
    void accept(PMSG pMsg);

    /**
     * Notifies socket of a connection attempt.
     *
     * @param pMsg The Windows message notifying of this event.
     *
     * @author Tom Nightingale
     */
    void connect(PMSG pMsg);

    /**
     * Set the socket to listen for connections on a specific port.
     *
     * @param port The port to listen to for connections.
     *
     * @author Tom Nightingale
     */
    bool listen(int port);

    /**
     * Connect to a remote machine.
     *
     * @param address The address of the machine to connect to.
     * @param port The port of the machine to connect to.
     *
     * @author Tom Nightingale
     */
    bool connectRemote(QString address, int port);

signals:
    /**
     * Emitted when a client connects to this listening socket.
     *
     * @param clientSocket The socket of the client that connected.
     *
     * @author Tom Nightingale
     */
    void signalClientConnected(Socket * clientSocket);

public slots:
    /**
     * All Windows Asynchronous socket message get sent here. The message is
     * parsed and then forwarded to the appropriate destination.
     *
     * @param socket The handle to the socket that received this message.
     * @param int The actual message received.
     *
     * @author Tom Nightingale.
     */
    void slotProcessWSAEvent(int, int);
};

#endif // TCPSOCKET_H
