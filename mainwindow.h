#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMap>
#include <QDebug>
#include <Phonon/MediaSource>
#include <Phonon/SeekSlider>
#include "tcpsocket.h"
#include "audiocomponent.h"
#include "joinserver.h"
#include "colors.h"
#include "ui_colors.h"
#include "ui_joinserver.h"
#include "remoteSong.h"
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

signals:
    void signalWMWSASyncTCPRx(int, int);
    void signalWMWSASyncUDPRx(int, int);
    void requestPlaylist(QString, short);
    void requestFile(QString, QString);

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
    bool muted_;
    QStringList songList_;
    QMap<QString,RemoteSong> remoteList_;

private slots:
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
    void on_addMusicButton_clicked();
    void on_playbackBox_valueChanged(double );
};

#endif // MAINWINDOW_H
