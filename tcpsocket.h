#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include "socket.h"

class TCPSocket : public Socket
{
    Q_OBJECT
public:
    TCPSocket(HWND hWnd);
    TCPSocket(SOCKET socket, HWND hWnd);

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
    void receive(PMSG pMsg);

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    void send(PMSG pMsg);

    /**
     *
     * @param pMsg
     *
     * @author Tom Nightingale
     */
    void connect(PMSG pMsg);

    /**
     *
     * @param bytesToRead
     *
     * @author Tom Nightingale
     */
    int loadBuffer(size_t bytesToRead);

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
};

#endif // TCPSOCKET_H
