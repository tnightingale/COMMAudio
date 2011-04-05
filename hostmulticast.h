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
    QStringList* getPlaylist();

private slots:
    void on_playButton_clicked();
    void loadLibrary(QStringList* library);

    void on_localLibraryList_itemDoubleClicked(QListWidgetItem* item);

signals:
    void startMulticast(QStringList *songs);
    void play();
    void pause();
private:
    Ui::HostMulticast *ui;
    QStringList library_;
    QStringList *playlist_;
};

#endif // HOSTMULTICAST_H
