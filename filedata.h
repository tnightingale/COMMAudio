#ifndef FILEDATA_H
#define FILEDATA_H

#include <QObject>
#include <QFile>

class filedata : public QObject
{
    Q_OBJECT
private:
    QString name_;
    QByteArray data_;

public:
    explicit filedata(QObject *parent = 0);
    explicit filedata(QObject *parent, QString name, QByteArray data);
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

    bool writeToFile();
    bool readFromFile();
signals:

public slots:

};

#endif // FILEDATA_H
