#include "filedata.h"
#include <QDataStream>
FileData::FileData(QObject *parent) :
    QObject(parent)
{
}

FileData::FileData(QObject *parent, QString name, short port) : QObject(parent){
    port_ = port;
    name_ = name;
}

FileData::FileData(QObject *parent, short port) : QObject(parent){
    port_ = port;
}

FileData::FileData(QObject *parent, QString name, QByteArray data, short port) : QObject(parent){
    name_ = name;
    data_ = data;
    port_ = port;
}

bool FileData::writeToFile() {
    int n = name_.lastIndexOf('/');
    int s = name_.size() - n - 1;
    QString songTitle = name_.right(s);
    QFile file(songTitle);
    if(!file.open(QIODevice::WriteOnly)){
        return false;
    }
    file.write(data_);
    file.close();
    return true;
}

bool FileData::readFromFile() {
    QFile file(name_);
    if(!file.open(QIODevice::ReadOnly)){
        return false;
    }
    data_ = file.readAll();
    file.close();
    return true;
}
