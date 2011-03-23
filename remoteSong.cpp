#include "remoteSong.h"

RemoteSong::RemoteSong(QString path, QString ip)
{
    path_ = path;
    ip_ = ip;
}

QString RemoteSong::getFilePath()
{
    return path_;
}

QString RemoteSong::getIp()
{
    return ip_;
}
