#include "multicastsession.h"
#include "audiocomponent.h"

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
        current_ = new QFile(playlistIterator_.next());
        if (!current_->open(QIODevice::ReadOnly)) {
            endSession();
        }
        header_ = current_->read(44);
    }

    nextBuff_ = generateBuffer();
}

QByteArray* MulticastSession::generateBuffer() {
    QByteArray* tempArray = new QByteArray;
    tempArray->append(header_);
    tempArray->append(current_->read(1024*8))
    current_->close();
    current_->deleteLater();
    current_=NULL;
}

void MulticastSession::endSession() {
    // End of stream.
    // Disconnect timer, delete later.
}
