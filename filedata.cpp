#include "filedata.h"
#include <QDataStream>
FileData::FileData(QObject *parent) :
    QObject(parent)
{
}

FileData::FileData(QObject *parent, QString path, short port) : QObject(parent){
    port_ = port;
    path_ = path;
}

FileData::FileData(QObject *parent, short port) : QObject(parent){
    port_ = port;
}

FileData::FileData(QObject *parent, QString path, QByteArray data, short port) : QObject(parent){
    path_ = path;
    data_ = data;
    port_ = port;
}

bool FileData::writeToFile() {
    int n = path_.lastIndexOf('/');
    int s = path_.size() - n - 1;
    QString songTitle = path_.right(s);
    QFile file(songTitle);
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }
    file.write(data_);
    file.close();
    return true;
}

bool FileData::readFromFile() {
    QFile file(path_);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }
    data_ = file.readAll();
    file.close();
    return true;
}
