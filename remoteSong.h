#ifndef REMOTESONG_H
#define REMOTESONG_H

#include <QString>
#include <QObject>

class RemoteSong
{
public:
    RemoteSong(){}
    RemoteSong(QString path, QString ip);
    ~RemoteSong(){}

    // Getters for input boxes
    QString getIp();
    QString getFilePath();

private:
    QString path_;
    QString ip_;
};
#endif // REMOTESONG_H
