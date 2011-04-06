#ifndef MULTICASTSESSION_H
#define MULTICASTSESSION_H

#include <QObject>
#include <QStringList>
#include <QByteArray>
#include <QFile>
#include <QTimerEvent>

class UDPSocket;

class MulticastSession : public QObject {
  Q_OBJECT
private:
    UDPSocket* multicastSocket_;
    QStringList* playlist_;
    QStringListIterator* playlistIterator_;
    QFile* current_;
    QByteArray* nextBuff_;
    QByteArray header_;
    int currentTimer_;

    /**
     *
     * @author Tom Nightingale
     */
    void loadBuffer();
    QByteArray* generateBuffer() ;

protected:
    void timerEvent(QTimerEvent* event);

public:
    MulticastSession(UDPSocket* socket, QStringList* playlist);
    virtual ~MulticastSession();

public slots:
    void start();
    void pause();
    void endSession();

signals:

};

#endif
