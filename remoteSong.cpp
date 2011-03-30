#include "remoteSong.h"

RemoteSong::RemoteSong(QString path, QString ip, short port)
{
    path_ = path;
    ip_ = ip;
    port_ = port;
}

QString RemoteSong::getFilePath()
{
    return path_;
}

QString RemoteSong::getIp()
{
    return ip_;
}

short RemoteSong::getPort()
{
    return port_;
}
