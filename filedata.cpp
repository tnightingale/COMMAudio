#include "filedata.h"
#include <QDataStream>
filedata::filedata(QObject *parent) :
    QObject(parent)
{
}

filedata::filedata(QObject *parent, QString name, QByteArray data) : QObject(parent){
    name_ = name;
    data_ = data;
}


bool filedata::writeToFile() {
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
bool filedata::readFromFile() {
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
