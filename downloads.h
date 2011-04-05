#ifndef DOWNLOADS_H
#define DOWNLOADS_H

#include <QDialog>

namespace Ui {
    class Downloads;
}

class Downloads : public QDialog
{
    Q_OBJECT

public:

    explicit Downloads(QWidget *parent = 0);
    ~Downloads();
    /**
     * amount of songs downloading
     *
     * @author Joel Stewart
     */
    int countCurrentDownloads();
    /**
     * download started
     *
     * @param filesize, size of file
     * @param packetsize, size of packet
     * @param file, name of file
     *
     * @author Joel Stewart
     */
    void downloadFile(int filesize, int packetSize, QString file);

private:
    Ui::Downloads *ui;
signals:
    /**
     * signals that queue is full
     *
     * @author Joel Stewart
     */
    void queueFull(bool full);
};

#endif // DOWNLOADS_H
