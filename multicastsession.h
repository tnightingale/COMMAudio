#ifndef MULTICASTSESSION_H
#define MULTICASTSESSION_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QByteArray>
#include <QFile>
#include "udpsocket.h"



class MulticastSession : public QObject {
  Q_OBJECT
private:
    UDPSocket* multicastSocket_;
    QStringList* playlist_;
    QTimer* timer_;
    QStringListIterator* playlistIterator_;
    QFile* current_;
    QByteArray* nextBuff_;
    QByteArray* header_;

    /**
     *
     * @author Tom Nightingale
     */
    void loadBuffer();
    QByteArray* generateBuffer() ;
public:
    MulticastSession(UDPSocket* socket, QStringList* playlist);





    virtual ~MulticastSession();

public slots:
    void start();
    void writeNextBuffer();
    void endSession();

signals:

};

#endif
