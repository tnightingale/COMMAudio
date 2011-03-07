#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QDebug>
#include "socket.h"

class UDPSocket : public Socket
{
    Q_OBJECT
private:
    SOCKADDR_IN serverSockAddrIn_;

public:
    UDPSocket(HWND hWnd);

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
     * @author Tom Nightingale.
     */
    void setDest(PHOSTENT host, size_t port) {
        memcpy((char*) &serverSockAddrIn_.sin_addr, host->h_addr, host->h_length);
        serverSockAddrIn_.sin_family = AF_INET;
        serverSockAddrIn_.sin_port = htons(port);
    }

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
        QString output;
        QTextStream log(&output, QIODevice::WriteOnly);

        int num = 0;
        PDATA data;

        if (error != 0) {
          qDebug("I/O operation failed with error %d\n", (int) error);
          return;
        }

        data = (PDATA) overlapped->hEvent;
        data->socket->updatePacketReceived(bytesTransferred);
        data->socket->updatePacketReceivedTime(GetTickCount());

        QDataStream * fileOutput = data->socket->getDataStream();
        if ((num = fileOutput->writeRawData(data->winsockBuff.buf, bytesTransferred)) < 0) {
            qDebug("STATIC UDPSocket::recvWorkerRoutine(): Error writing to file.");
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

        char* buff = (char*) overlapped->hEvent;
        free(buff);
        free(overlapped);
    }
};

#endif // UDPSOCKET_H
