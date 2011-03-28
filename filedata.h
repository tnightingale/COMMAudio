#ifndef FILEDATA_H
#define FILEDATA_H

#include <QObject>
#include <QFile>

class FileData : public QObject
{
    Q_OBJECT



private:
    QString name_;
    QByteArray data_;

public:
    explicit FileData(QObject *parent = 0);
    explicit FileData(QObject *parent, QString name, QByteArray data);
    void setName(QString name) {
        name_ = name;
    }
    QString getName() {
        return name_;
    }
    void setData(QByteArray data) {
        data_ = data;
    }
    QByteArray getData() {
        return data_;
    }

    void append(QByteArray data) {
        data_.append(data);
    }

    bool writeToFile();
    bool readFromFile();



signals:

public slots:

};


#endif // FILEDATA_H
