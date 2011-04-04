#include "multicastsession.h"

MulticastSession::MulticastSession(UDPSocket* socket, QStringList* playlist)
: multicastSocket_(socket), playlist_(playlist), timer_(new QTimer(this)), 
  playlistIterator_(playlist_), current_(NULL), nextBuff_(NULL) {
    connect(timer_, SIGNAL(timeout()),
            this, SLOT(writeNextBuffer()));
    multicastSocket_->setDest(multicastAddr, 0);
}

void MulticastSession::start() {
    loadBuffer();
    timer_->start(50);
}

void MulticastSession::writeNextBuffer() {
    multicastSocket_->write(*nextBuff_);
    delete nextBuff_;
    nextBuff_ = NULL;
    loadBuffer();
}

void MulticastSession::loadBuffer() {
    if (current_ == NULL) {
        if (!playlistIterator_.hasNext()) {
            endSession();
        }
        current_ = new Qfile(playlistIterator_.next());
        if (!current_->open(QIODevice::ReadOnly)) {
            endSession();
        }
    }

    nextBuff_ = generateBuffer();
}

QByteArray* MulticastSession::generateBuffer() {

}

void MulticastSession::endSession() {
    // End of stream.
    // Disconnect timer, delete later.
}
