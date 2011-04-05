#include "multicastsession.h"


MulticastSession::MulticastSession(UDPSocket* socket, QStringList* playlist)
: multicastSocket_(socket), playlist_(playlist), timer_(new QTimer(this)), 
  playlistIterator_(new QStringListIterator(*playlist_)),current_(NULL), nextBuff_(NULL) {
    connect(timer_, SIGNAL(timeout()),
            this, SLOT(writeNextBuffer()));
    QString multicastAddr("234.5.6.7");
    multicastSocket_->listen(0);
    multicastSocket_->setDest(multicastAddr, 7000);
}

MulticastSession::~MulticastSession(){}

void MulticastSession::start() {
    playlistIterator_=new QStringListIterator(*playlist_);
    //playlistIterator_(*playlist_);
    loadBuffer();
    timer_->start(50);
}

void MulticastSession::writeNextBuffer() {
    QStringList* tempplaylist= playlist_;
    multicastSocket_->write(*nextBuff_);
    delete nextBuff_;
    nextBuff_ = NULL;
    loadBuffer();
}

void MulticastSession::loadBuffer() {
    if (current_ == NULL) {
        if (!playlistIterator_->hasNext()) {
            endSession();
            return;
        }
        //QString nameeeu = playlistIterator_->next();
        current_ = new QFile(playlistIterator_->next());
        if (!current_->open(QIODevice::ReadOnly)) {
            endSession();
            return;
        }
        header_ = current_->read(44);
    }

    nextBuff_ = generateBuffer();
}

QByteArray* MulticastSession::generateBuffer() {
    QByteArray* tempArray = new QByteArray;
    tempArray->append(header_);
    tempArray->append(current_->read(1024*8));
    /*if(tempArray->size() != 1024*8 + 44){

    }*/
    if(current_->atEnd()){
        current_->close();
        current_->deleteLater();
        current_=NULL;
    }
    return tempArray;
}

void MulticastSession::endSession() {
    timer_->stop();
    disconnect(timer_, SIGNAL(timeout()),
               this, SLOT(writeNextBuffer()));
    timer_->deleteLater();

    // End of stream.
    // Disconnect timer, delete later.
}
