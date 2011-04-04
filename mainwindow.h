#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMap>
#include <QDebug>
#include "tcpsocket.h"
#include "audiocomponent.h"
#include "joinserver.h"
#include "colors.h"
#include "ui_colors.h"
#include "ui_joinserver.h"
#include "remoteSong.h"
#include "downloads.h"
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimeLine>
#include <QGraphicsItemAnimation>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString findFullPath(QString filename);

    /**
     * Returns the window's Ui interface.
     *
     * @author Tom Nightingale
     */
    Ui::MainWindow* getUi() { return ui; }

    /**
     * Expose the windows message loop for us to respond to socket events.
     * The first of many attrocities...
     *
     * @param msg The windows message received.
     * @param result ... not sure >_>
     *
     * @author Tom Nightingale
     */
    bool winEvent(PMSG msg, long * result);

    /**
     * adds to the QMap
     *
     * @param songList, the song list recieved
     * @param ipAddress, ip address where fileList was revieved from
     *
     * @author Joel Stewart
     */
    void appendToRemote(QStringList songList, QString ipAddress, short port);

    void visualization(int n);

    QStringList getLocalFileList();

    void backgroundColor(QString background, QString font);

    void updateMusicContent(QStringList currentSongs);

    void updateClientlist();

    void loadlastPlaylist();

    void savePlaylist();

    void updatePlaylist();

    void addSongToLocal(QString song);

    bool requestVoiceChat(QString);

signals:
    void signalWMWSASyncTCPRx(int, int);
    void signalWMWSASyncUDPRx(int, int);
    void requestPlaylist(QString, short);
    void requestFile(QString,short, QString);

private:
    QSlider *slider_;
    Ui::MainWindow *ui;
    TCPSocket *controlSocket_;
    AudioComponent* player_;
    QMediaPlayer* playerlink_;
    JoinServer joinServer_;
    JoinServer requestPlaylist_;
    Colors changeColor_;
    QTimeLine *timer_;
    QMediaPlaylist* playlist_;
    bool muted_;
    QStringList songList_;
    QStringList playlistData_;
    QMap<QString,RemoteSong> remoteList_;
    Downloads downloads_;

private slots:
    void playlistIndexChanged(int index);
    void seek(int seconds);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void on_action_Join_Multicast_triggered();
    void on_action_Modify_triggered();
    void on_action_Request_Playlist_triggered();
    void on_action_Visible_toggled(bool status);
    void on_clientListWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_playButton_clicked();
    void on_stopButton_clicked();
    void on_remoteListWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_talkButton_pressed();
    void on_talkButton_released();
    void on_tabWidget_currentChanged(int index);
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void on_muteToolButton_clicked();
    void on_horizontalSlider_valueChanged(int volume);
    void on_playlistWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_playbackBox_valueChanged(double );
    void on_action_Advanced_toggled(bool status);
    void on_savePlaylistButton_clicked();
    void on_loadPlaylistButton_clicked();
    void on_clearPlaylistButton_clicked();
    void on_clearLocalButton_clicked();
    void on_clearRemoteButton_clicked();
    void on_removeButton_clicked();
    void on_playlistWidget_currentRowChanged(int currentRow);
    void on_action_Folder_triggered();
    void on_action_Song_triggered();
    void on_action_Tiger_triggered();
    void on_action_Default_triggered();
    void downloadStarted(int filesize, int packsizeRecv, QString file);
    void on_viewDownloadButton_clicked();
};

#endif // MAINWINDOW_H
