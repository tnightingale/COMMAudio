#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QThread>

class ListenThread : public QThread
{
    Q_OBJECT
public:
    void run();

public slots:
    void closeListenThread();
};

#endif // LISTENTHREAD_H
