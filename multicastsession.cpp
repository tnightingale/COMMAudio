#include "multicastsession.h"
#include "udpsocket.h"


MulticastSession::MulticastSession(UDPSocket* socket, QStringList* playlist)
: multicastSocket_(socket), playlist_(playlist),
  playlistIterator_(new QStringListIterator(*playlist_)), current_(NULL), nextBuff_(NULL),
  currentTimer_(0) {
    QString multicastAddr("234.5.6.8");
    multicastSocket_->listen(0);
    multicastSocket_->setDest(multicastAddr, 7000);
}

MulticastSession::~MulticastSession(){}

void MulticastSession::start() {
    playlistIterator_ = new QStringListIterator(*playlist_);
    loadBuffer();
    currentTimer_ = startTimer(35);  //timer_->start(35);
}

void MulticastSession::pause() {
    killTimer(currentTimer_);
}

void MulticastSession::timerEvent(QTimerEvent* event) {
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
    qDebug("Multicastsession::closeSession(); closed.");
    //timer_->stop();
    //disconnect(timer_, SIGNAL(timeout()),
    //           this, SLOT(writeNextBuffer()));
    //timer_->deleteLater();

    // End of stream.
    // Disconnect timer, delete later.
}
