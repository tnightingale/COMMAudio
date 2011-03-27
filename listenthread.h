#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QThread>
#include "mainwindow.h"
#include "ui_mainwindow.h"

class ListenThread : public QThread
{
    Q_OBJECT
public:
    void run();

private:

public slots:
    void closeListenThread();
};

#endif // LISTENTHREAD_H
