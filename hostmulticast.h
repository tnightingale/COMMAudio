#ifndef HOSTMULTICAST_H
#define HOSTMULTICAST_H

#include <QDialog>
#include <QListWidgetItem>
#include <QDebug>
namespace Ui {
    class HostMulticast;
}

class HostMulticast : public QDialog
{
    Q_OBJECT

public:
    explicit HostMulticast(QWidget *parent = 0);
    ~HostMulticast();
    QString findFullPath(QString filename);

private slots:
    /**
     * starts multicast
     *
     * @author Joel Stewart
     */
    void on_playButton_clicked();
    /**
     * loads "*.wav" from library of local
     *
     * @author Joel Stewart
     */
    void loadLibrary(QStringList* library);

    /**
     * adds item to playlist
     *
     * @param item, item double-clicked
     *
     * @author Joel Stewart
     */
    void on_localLibraryList_itemDoubleClicked(QListWidgetItem* item);

signals:
    /**
     * signal of song list
     *
     * @param songs, playlist of songs
     *
     * @author Joel Stewart
     */
    void startMulticast(QStringList *songs);
    /**
     * play clicked
     *
     * @author Joel Stewart
     */
    void play();
    /**
     * pause clicked
     *
     * @author Joel Stewart
     */
    void pause();
private:
    Ui::HostMulticast *ui;
    QStringList library_;
    QStringList *playlist_;
};

#endif // HOSTMULTICAST_H
