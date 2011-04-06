#ifndef FILEDATA_H
#define FILEDATA_H

#include <QObject>
#include <QFile>

class FileData : public QObject
{
    Q_OBJECT

private:
    /**
     * Holds the file path of the file on disk
     *
     * @author Luke Queenan
     */
    QString path_;

    /**
     * Holds the actual bytes of the file
     *
     * @author Luke Queenan
     */
    QByteArray data_;

    /**
     * Holds the port associated with the client this file was retrieved from
     *
     * @author Luke Queenan
     */
    short port_;

    /**
     * The complete size of the file when stored on disk
     *
     * @author Luke Queenan
     */
    int totalSize_;

public:
    explicit FileData(QObject *parent = 0);
    explicit FileData(QObject *parent, QString path, short port);
    explicit FileData(QObject *parent, short port);
    explicit FileData(QObject *parent, QString path, QByteArray data, short port);

    /**
     * Allows setting of the path
     *
     * @author Luke Queenan
     */
    void setpath(QString path) {
        path_ = path;
    }

    /**
     * Returns the path
     *
     * @author Luke Queenan
     */
    QString getPath() {
        return path_;
    }

    /**
     * Allows a complete file to be loaded into memory
     *
     * @author Luke Queenan
     */
    void setData(QByteArray data) {
        data_ = data;
    }

    /**
     * Returns the file
     *
     * @author Luke Queenan
     */
    QByteArray getData() {
        return data_;
    }

    /**
     * Sets the port
     *
     * @author Luke Queenan
     */
    void setPort(short port) {
        port_ = port;
    }

    /**
     * Returns the port
     *
     * @author Luke Queenan
     */
    short getPort() {
        return port_;
    }

    /**
     * Sets the total size of the file
     *
     * @author Luke Queenan
     */
    void setTotalSize(int size) {
        totalSize_ = size;
    }

    /**
     * Returns the total size of the file
     *
     * @author Luke Queenan
     */
    int getTotalSize() {
        return totalSize_;
    }

    /**
     * Allows data to be appended to the internal data. Used for receiving files
     * as packets.
     *
     * @author Luke Queenan
     */
    void append(QByteArray data) {
        data_.append(data);
    }

    /**
     * Checks if the current size of the internal data array equals the
     * specified total size and returns the true false result.
     *
     * @author Luke Queenan
     */
    bool transferComplete() {
        return (data_.size() == totalSize_);
    }

    /**
     * Writes the file out to disk. The file is written to the disk using the
     * file name in the path
     *
     * @author Luke Queenan
     */
    bool writeToFile();

    /**
     * Opens and reads the file into the internal data array using the file path
     * information.
     *
     * @author Luke Queenan
     */
    bool readFromFile();



signals:

public slots:

};


#endif // FILEDATA_H
