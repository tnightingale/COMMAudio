#include "socket.h"

Socket::Socket(HWND hWnd, int addressFamily, int connectionType, int protocol)
: hWnd_(hWnd) {
    if ((socket_ = WSASocket(addressFamily, connectionType, protocol, NULL, 0,
                             WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        int err = WSAGetLastError();
        qDebug("Socket::Socket(): Can't create socket. (%d)", err);
        throw "Socket::Socket(); Can't create socket.";
    }
}

bool Socket::listen(PSOCKADDR_IN pSockAddr) {
    QString output;
    QTextStream log(&output, QIODevice::WriteOnly);

    int err = 0;

    if ((err = bind(socket_, (PSOCKADDR) pSockAddr, sizeof(SOCKADDR))
        == SOCKET_ERROR)) {
        qDebug("Socket::startServer(): Can't bind to socket. Error: %d",
               WSAGetLastError());
        return false;
    }

    log << "Bound socket " << (int) socket_
        << " to port " << (int) ntohs(pSockAddr->sin_port) << ".";
    outputStatus(output);

    // TODO: Might want to call linger here.
    // allow the TCP packets to linger for up to 10 seconds
    //LINGER* linger = (LINGER*) malloc(sizeof(LINGER));
    //linger->l_onoff = 1;
    //linger->l_linger = 2;
    //setsockopt(socket_, SOL_SOCKET, SO_LINGER, (char*) linger, sizeof(linger));

    return true;
}

void Socket::close(PMSG pMsg) {
    QString output;
    QTextStream log(&output, QIODevice::WriteOnly);

    log << "Socket::close(); " << (int) pMsg->wParam << " disconnected.";
    outputStatus(output);

    emit signalStatsSetFinishTime(GetTickCount());

    shutdown(pMsg->wParam, SD_SEND);
    emit signalSocketClosed();
}



bool Socket::slotProcessWSAEvent(PMSG pMsg) {
    if (WSAGETSELECTERROR(pMsg->lParam)) {
        qDebug("Socket::slotProcessWSAEvent(): %d: Socket failed with error %d",
              (int) pMsg->wParam, WSAGETSELECTERROR(pMsg->lParam));
        return false;
    }

    switch (WSAGETSELECTEVENT(pMsg->lParam)) {

        case FD_CLOSE:
            qDebug("Socket::slotProcessWSAEvent: %d: FD_CLOSE.", (int) pMsg->wParam);
            close(pMsg);
            break;
    }

    return true;
}

void Socket::updatePacketReceived(int bytes) {
    emit signalStatsSetBytes(bytes);
    emit signalStatsSetPackets(1);
}

void Socket::updatePacketReceivedTime(int time) {
    emit signalStatsSetFinishTime(time);
}
