#include "udpsocket.h"

UDPSocket::UDPSocket(HWND hWnd) 
: Socket(hWnd, AF_INET, SOCK_DGRAM, IPPROTO_UDP) {
    int err = 0;
    int flags = FD_READ | FD_CLOSE;

    if ((err = WSAAsyncSelect(socket_, hWnd, WM_WSAASYNC_TCP, flags))
                              == SOCKET_ERROR) {
        qDebug("UDPSocket::UDPSocket(): Error setting up async select.");
        throw "UDPSocket::UDPSocket(): Error setting up async select.";
    }
}

void UDPSocket::send(PMSG pMsg) {
    int err = 0;
    int result = 0;
    int num = 0;
    size_t bytesRead = 0;
    size_t bytesToRead = getPacketSize();
    size_t totalSent = 0;

    DWORD numBytesSent = 0;
    WSAOVERLAPPED* ol;
    WSABUF winsockBuff;

    winsockBuff.len = getPacketSize();
    bytesRead = winsockBuff.len;

/*
    while (data_->status() == QDataStream::Ok) {
        ol = (WSAOVERLAPPED*) calloc(1, sizeof(WSAOVERLAPPED));
        winsockBuff.buf = (char *) malloc(bytesToRead * sizeof(char));
        ol->hEvent = (HANDLE) winsockBuff.buf;

        if ((num = data_->readRawData(winsockBuff.buf, bytesToRead)) <= 0) {
            qDebug("UDPSocket()::send(); Finishing...");
            break;
        }
        winsockBuff.len = num;
        totalSent += num;

        result = WSASendTo(pMsg->wParam, &winsockBuff, 1, &numBytesSent, 0,
                           (PSOCKADDR) &serverSockAddrIn_, sizeof(serverSockAddrIn_), ol,
                           UDPSocket::sendWorkerRoutine);

        if ((err = WSAGetLastError()) > 0 && err != ERROR_IO_PENDING) {
            qDebug("UDPSocket::send(); Error: %d", err);
            return;
        }

        if (result == WSAEWOULDBLOCK) {
            return;
        }

    }
*/
}

void UDPSocket::receive(PMSG pMsg) {
    int err = 0;
    DWORD flags = 0;
    WSAOVERLAPPED* ol;

    PDATA data = (PDATA) calloc(1, sizeof(DATA));
    data->socket = this;
    data->winsockBuff.len = MAXUDPDGRAMSIZE;
    data->winsockBuff.buf = (char*) calloc(data->winsockBuff.len, sizeof(char));
    data->clientSD = pMsg->wParam;

    ol = (WSAOVERLAPPED*) calloc(1, sizeof(WSAOVERLAPPED));
    ol->hEvent = (HANDLE) data;

    if (WSARecvFrom(pMsg->wParam, &(data->winsockBuff), 1, NULL, &flags,
                NULL, NULL, ol, UDPSocket::recvWorkerRoutine) == SOCKET_ERROR) {
        if ((err = WSAGetLastError()) != WSA_IO_PENDING) {
            qDebug("UDPSocket::receive(): WSARecv() failed with error %d",
                   err);
            return;
        }
    }
}

bool UDPSocket::slotProcessWSAEvent(PMSG pMsg) {
    if (WSAGETSELECTERROR(pMsg->lParam)) {
        qDebug("UDPSocket::slotProcessWSAEvent(): %d: Socket failed. Error: %d",
              (int) pMsg->wParam, WSAGETSELECTERROR(pMsg->lParam));
        return false;
    }

    // Filtering out messages for other sockets / protocols.
    if (pMsg->wParam != socket_) {
        return false;
    }

    switch (WSAGETSELECTEVENT(pMsg->lParam)) {
        case FD_READ:
            qDebug("UDPSocket::slotProcessWSAEvent: %d: FD_READ.",
                   (int) pMsg->wParam);
            receive(pMsg);
            break;

        case FD_WRITE:
            qDebug("UDPSocket::slotProcessWSAEvent: %d: FD_WRITE.",
                   (int) pMsg->wParam);
            send(pMsg);
            break;

        default:
            Socket::slotProcessWSAEvent(pMsg);
            break;
    }

    return;
}
