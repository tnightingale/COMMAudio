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
    int countCurrentDownloads();
    void downloadFile(int filesize, int packetSize, QString file);

private:
    Ui::Downloads *ui;
signals:
    void queueFull(bool full);
};

#endif // DOWNLOADS_H
