#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QDebug>
#include "socket.h"
#include <ws2tcpip.h>

/**
 * Wrapper class for a Windows asynchronous UDP socket. Base socket
 * functionality is provided by parent class Socket.
 *
 * @author Tom Nightingale
 */
class UDPSocket : public Socket
{
    Q_OBJECT

private:
    /** The address of the destination machine. */
    SOCKADDR_IN serverSockAddrIn_;

    /** Whether or not this is a multicast socket. */
    bool isMulticast_;

public:
    /**
     * Constructor.
     *
     * @param hWnd The pointer to the applications mainwindow for access to the
     *             applications message loop.
     *
     * @author Tom Nightingale
     */
    UDPSocket(HWND hWnd);

    /**
     * Destructor.
     * Cleans up any resources used by the socket.
     *
     * @author Tom Nightingale
     */
    virtual ~UDPSocket();

    /**
     * Reimplemented from QIODevice.
     * Sets this IO device's open mode to the mode specified.
     * 
     * @param mode The mode to set this device to.
     *
     * @author Tom Nightingale
     */
    bool open(OpenMode mode);

    /**
     * Set the socket to listen for datagrams on a specific port.
     *
     * @param port The port to listen to for datagrams.
     *
     * @author Tom Nightingale
     */
    virtual bool listen(int port);

    /**
     * Set the socket to listen for multicast datagrams on a specific port.
     * 
     * @param address The address to listen to for multicast datagrams.
     * @param port The port to listen to for multicast datagrams.
     *
     * @author Tom Nightingale
     */
    void listenMulticast(QString address, int port);

    /**
     * Set the destination address of this port.
     *
     * @param hostAddr The address of the remote machine.
     * @param param The port of the remote machine.
     *
     * @author Tom Nightingale.
     */
    void setDest(QString hostAddr, size_t port);

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
    void slotProcessWSAEvent(int socket, int lParam);

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
};

#endif // UDPSOCKET_H
