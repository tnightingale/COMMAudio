#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QDebug>
#include "socket.h"

class TCPSocket : public Socket
{
    Q_OBJECT
public:
    TCPSocket(HWND hWnd);

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

    void connect(PMSG pMsg);

    int loadBuffer(size_t bytesToRead);

    /**
     *
     * @param pSockAddr
     *
     * @author Tom Nightingale
     */
    bool listen(PSOCKADDR_IN pSockAddr);

    /**
     *
     * @param pSockAddr
     *
     * @author Tom Nightingale
     */
    bool connectRemote(PSOCKADDR_IN pSockAddr);

public slots:
    /**
     *
     * @param pMsg
     *
     * @return
     *
     * @author Tom Nightingale.
     */
    bool slotProcessWSAEvent(PMSG pMsg);

public:
    /**
     *
     * @param error
     *
     * @author Tom Nightingale
     */
    static void CALLBACK recvWorkerRoutine(DWORD error, DWORD bytesTransferred,
                                           LPWSAOVERLAPPED overlapped,
                                           DWORD inFlags) {
        int num = 0;
        PDATA data;

        if (error != 0) {
          qDebug("I/O operation failed with error %d\n", (int) error);
          return;
        }

        data = (PDATA) overlapped->hEvent;
        data->socket->updatePacketReceived(bytesTransferred);

        QDataStream * fileOutput = data->socket->getDataStream();
        if ((num = fileOutput->writeRawData(data->winsockBuff.buf, bytesTransferred)) < 0) {
            qDebug("STATIC TCPSocket::recvWorkerRoutine(): Error writing to file.");
        }

        QFile * file = (QFile *) fileOutput->device();
        if (!file->flush()) qDebug("error flushing file");

        free(data);
        free(overlapped);
    }

    /**
     *
     * @param error
     *
     * @author Tom Nightingale
     */
    static void CALLBACK sendWorkerRoutine(DWORD error, DWORD bytesTransferred,
                                           LPWSAOVERLAPPED overlapped,
                                           DWORD inFlags) {
        if (error != 0) {
          qDebug("I/O operation failed with error %d\n", (int) error);
          return;
        }

        free(overlapped);
    }
};

#endif // TCPSOCKET_H
