#ifndef IOMANAGER_H
#define IOMANAGER_H

#include <QObject>

class IOManager : public QObject {
  Q_OBJECT
private:
    SOCKET socketD_;

public:
    IOManager(SOCKET socketD);
    virtual ~IOManager();

public slots:
    void beginWrite(QIODevice source

};

#endif
