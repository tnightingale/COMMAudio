#ifndef MULTICASTSESSION_H
#define MULTICASTSESSION_H

#include <QObject>
#include <QStringList>
#include <QTimer>
#include <QByteArray>

class MulticastSession : public QObject {
  Q_Object
private:
    UDPSocket* multicastSocket_;
    QStringList* playlist_;
    QTimer* timer_;
    QStringListIterator playlistIterator_;
    QFile* current_;
    QByteArray* nextBuff_;

    /**
     *
     * @author Tom Nightingale
     */
    void loadBuffer();

public:
    MulticastSession(UDPSocket* socket, QStringList* playlist);
    virtual ~MulticastSession();

public slots:
    void start();
    void writeNextBuffer();
    void endSession();

signals:

}

#endif
