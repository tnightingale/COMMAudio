#ifndef REMOTESONG_H
#define REMOTESONG_H

#include <QString>
#include <QObject>

class RemoteSong
{
public:
    RemoteSong(){}
    RemoteSong(QString path, QString ip, short port);
    ~RemoteSong(){}

    // Getters for input boxes
    /**
     * returns ip
     *
     * @author Joel Stewart
     */
    QString getIp();
    /**
     * returns filepath
     *
     * @author Joel Stewart
     */
    QString getFilePath();
    /**
     * returns port
     *
     * @author Joel Stewart
     */
    short getPort();

private:
    QString path_;
    QString ip_;
    short port_;
};
#endif // REMOTESONG_H
