#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include "socket.h"

class TCPSocket : public Socket
{
    Q_OBJECT

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

public:
    TCPSocket(HWND hWnd);
    TCPSocket(SOCKET socket, HWND hWnd);

    virtual bool open(OpenMode mode);

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    void accept(PMSG pMsg);

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    void connect(PMSG pMsg);

    /**
     *
     * @param pSockAddr
     *
     * @author Tom Nightingale
     */
    bool listen(int port);

    /**
     *
     * @param pSockAddr
     *
     * @author Tom Nightingale
     */
    bool connectRemote(QString address, int port);

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

signals:
    /**
     *
     * @param clientSocket
     *
     * @author Tom Nightingale
     */
    void signalClientConnected(TCPSocket * clientSocket);

    /**
     *
     * @param socket
     * @param buffer
     *
     * @author Tom Nightingale
     */
    void signalDataReceived(TCPSocket *tcpSocket);

public slots:
    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale.
     */
    //void slotProcessWSAEvent(PMSG pMsg);
    void slotProcessWSAEvent(int, int);

private:
    QString connectedIp_;
    short connectedPort_;
};

#endif // TCPSOCKET_H
