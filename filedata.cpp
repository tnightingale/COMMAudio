#include "filedata.h"
#include <QDataStream>
FileData::FileData(QObject *parent) :
    QObject(parent)
{
}

FileData::FileData(QObject *parent, QString name, QByteArray data) : QObject(parent){
    name_ = name;
    data_ = data;
}

bool FileData::writeToFile() {
    QString path = "./" + name_;
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)){
        return FALSE;
    }
    QDataStream stream(&file);
    stream << data_;
    file.close();
    return TRUE;
}

bool FileData::readFromFile() {
    QString path = "./" + name_;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)){
        return FALSE;
    }
    QDataStream stream(&file);
    stream >> data_;
    file.close();
    return TRUE;
}
