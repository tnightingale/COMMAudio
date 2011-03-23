#include "socket.h"

Socket::Socket(HWND hWnd, int addressFamily, int connectionType, int protocol)
: data_(NULL), hWnd_(hWnd) {
    if ((socket_ = WSASocket(addressFamily, connectionType, protocol, NULL, 0,
                             WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
        int err = WSAGetLastError();
        qDebug("Socket::Socket(): Can't create socket. (%d)", err);
        throw "Socket::Socket(); Can't create socket.";
    }

    connect(this, SIGNAL(signalSocketClosed()),
            this, SLOT(deleteLater()));
}

Socket::Socket(SOCKET socket, HWND hWnd) {
    socket_ = socket;
    hWnd_ = hWnd;
}

bool Socket::listen(PSOCKADDR_IN pSockAddr) {
    int err = 0;

    if ((err = bind(socket_, (PSOCKADDR) pSockAddr, sizeof(SOCKADDR))
        == SOCKET_ERROR)) {
        qDebug("Socket::startServer(): Can't bind to socket. Error: %d",
               WSAGetLastError());
        return false;
    }

    return true;
}

void Socket::close(PMSG pMsg) {
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
