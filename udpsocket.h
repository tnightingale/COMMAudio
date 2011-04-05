#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QDebug>
#include "socket.h"
#include <ws2tcpip.h>

class UDPSocket : public Socket
{
    Q_OBJECT
private:
    SOCKADDR_IN serverSockAddrIn_;

public:
    UDPSocket(HWND hWnd);

    bool open(OpenMode mode);

    virtual bool listen(int port);
    void listenMulticast(QString address, int port);

    /**
     *
     * @author Tom Nightingale.
     */
    void setDest(QString hostAddr, size_t port);

public slots:
    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale.
     */
    void slotProcessWSAEvent(int socket, int lParam);

protected:
    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    virtual void receive(PMSG pMsg);

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    virtual void send(PMSG pMsg);
};

#endif // UDPSOCKET_H
