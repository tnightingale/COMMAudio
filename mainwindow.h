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
#include "hostmulticast.h"
#include "joinmulticast.h"
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

    /**
     * gets full path of song
     *
     * @param filename, song title
     *
     * @author Joel Stewart
     */
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

    AudioComponent* getAudioPlayer() { return player_; }

    /**
     * adds to the visualization
     *
     * @param n, size of bars in visual window
     *
     * @author Joel Stewart
     */
    void visualization(int n);
    HostMulticast* getHostMulticast() { return multicast_; }
    JoinMulticast* getJoinMulticast() { return joinMulticast_; }

    /*
    -- FUNCTION: getLocalFileList
    --
    -- DATE: March 21, 2011
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Luke Queenan
    --
    -- PROGRAMMER: Luke Queenan
    --
    -- INTERFACE: QStringList MainWindow::getLocalFileList()
    --
    -- RETURNS: The local filelist stored in the audio player
    --
    -- NOTES:
    -- Gets the local filelist stored in the audio player in main window.
    */
    QStringList getLocalFileList();

    /**
     * adds a theme to main window
     *
     * @param background, color of background
     * @param font, font color
     *
     * @author Joel Stewart
     */
    void backgroundColor(QString background, QString font);

    /**
     * updates music list
     *
     * @param currentSongs, current list of songs
     *
     * @author Joel Stewart
     */
    void updateMusicContent(QStringList currentSongs);

    /**
     * updates clientlist of music content
     *
     *
     * @author Joel Stewart
     */
    void updateClientlist();

    /**
     * loads choosen playlist
     *
     * @author Joel Stewart
     */
    void loadlastPlaylist();

    /**
     * saves playlist
     *
     *
     * @author Joel Stewart
     */
    void savePlaylist();

    /**
     * updates list of songs currently in playlist
     *
     * @author Joel Stewart
     */
    void updatePlaylist();

    /**
     * add individual song to clientlist
     *
     * @param song, song thats going to be added
     *
     * @author Joel Stewart
     */
    void addSongToLocal(QString song);

    bool requestVoiceChat(QString);

    bool getVoiceCallActive();

    void setVoiceCallActive(bool);

signals:
    void signalWMWSASyncTCPRx(int, int);
    void signalWMWSASyncUDPRx(int, int);
    void requestPlaylist(QString, short);
    void requestFile(QString,short, QString);
    void initiateVoiceStream(short port, QString hostAddr, AudioComponent* player);
    void disconnectVoiceStream();
    void voicePressed(AudioComponent*);
    void voiceReleased(AudioComponent*);
    /**
     * signal for hosting a multicast
     *
     * @param songs, songs the client knows about
     *
     * @author Joel Stewart
     */
    void multicastList(QStringList *songs);
    void startMulticast(QStringList* list);


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
    bool voiceCallActive_;
    QStringList songList_;
    QStringList playlistData_;
    QMap<QString,RemoteSong> remoteList_;
    Downloads downloads_;
    HostMulticast *multicast_;
    JoinMulticast *joinMulticast_;

private slots:
    void on_talkButton_clicked();
    void playlistIndexChanged(int index);
    /**
     * seek bar for song playing
     *
     * @param seconds, using seconds of song
     *
     * @author Joel Stewart
     */
    void seek(int seconds);
    /**
     * the song duration changed
     *
     * @param duration, adjust the seek bar
     *
     * @author Joel Stewart
     */
    void durationChanged(qint64 duration);
    /**
     * position of seek bar changed
     *
     * @param progress, adjust seek bar accordingly
     *
     * @author Joel Stewart
     */
    void positionChanged(qint64 progress);
    /**
     * pop up window to join multicast
     *
     *
     * @author Joel Stewart
     */
    void on_action_Join_Multicast_triggered();
    /**
     * pops uo window for custom theme
     *
     *
     * @author Joel Stewart
     */
    void on_action_Modify_triggered();
    /**
     * pops up window to request a playlist
     *
     *
     * @author Joel Stewart
     */
    void on_action_Request_Playlist_triggered();
    void on_action_Visible_toggled(bool status);
    /*
    -- FUNCTION: on_clientListWidget_itemDoubleClicked
    --
    -- DATE: March 21, 2011
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Joel Stewart
    --
    -- PROGRAMMER: Joel Stewart
    --
    -- INTERFACE: void MainWindow::on_clientListWidget_itemDoubleClicked(QListWidgetItem* item)
    --
    -- RETURNS:
    --
    -- NOTES:
    -- allows clicking on a song in the item list
    */
    void on_clientListWidget_itemDoubleClicked(QListWidgetItem* item);
    /*
    -- FUNCTION: on_playButton_clicked
    --
    -- DATE: March 21, 2011
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Joel Stewart
    --
    -- PROGRAMMER: Joel Stewart
    --
    -- INTERFACE: void MainWindow::on_playButton_clicked()
    --
    -- RETURNS:
    --
    -- NOTES:
    -- Plays song
    */
    void on_playButton_clicked();
    /*
    -- FUNCTION: on_stopButton_clicked
    --
    -- DATE: March 21, 2011
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Joel Stewart
    --
    -- PROGRAMMER: Joel Stewart
    --
    -- INTERFACE: void MainWindow::on_stopButton_clicked()
    --
    -- RETURNS:
    --
    -- NOTES:
    -- Stops song
    */
    void on_stopButton_clicked();

    /*
    -- FUNCTION: on_remoteListWidget_itemDoubleClicked
    --
    -- DATE: March 21, 2011
    --
    -- REVISIONS: (Date and Description)
    --
    -- DESIGNER: Joel Stewart
    --
    -- PROGRAMMER: Joel Stewart
    --
    -- INTERFACE: void MainWindow::on_remoteListWidget_itemDoubleClicked(QListWidgetItem* item)
    --
    -- RETURNS:
    --
    -- NOTES:
    -- combined list of clients
    */
    void on_remoteListWidget_itemDoubleClicked(QListWidgetItem* item);
    /**
     * current tab in use
     *
     * @param index, index of page
     *
     * @author Joel Stewart
     */
    void on_tabWidget_currentChanged(int index);
    /**
     * goes to next song
     *
     * @author Joel Stewart
     */
    void on_nextButton_clicked();
    /**
     * goes to previous song
     *
     * @author Joel Stewart
     */
    void on_previousButton_clicked();
    /**
     * mutes song
     *
     * @author Joel Stewart
     */
    void on_muteToolButton_clicked();
    /**
     * adjusts volume
     *
     * @param volume, current volume of song
     *
     * @author Joel Stewart
     */
    void on_horizontalSlider_valueChanged(int volume);
    /**
     * playlist item double-clicked
     *
     * @param item, item clicked
     *
     * @author Joel Stewart
     */
    void on_playlistWidget_itemDoubleClicked(QListWidgetItem* item);
    /**
     * adjusts playback rate
     *
     * @param double, value of playback rate
     *
     * @author Joel Stewart
     */
    void on_playbackBox_valueChanged(double );
    /**
     * adjusts view
     *
     * @param status, if advanced view is checked
     *
     * @author Joel Stewart
     */
    void on_action_Advanced_toggled(bool status);
    /**
     * pops up save playlist dialog
     *
     *
     * @author Joel Stewart
     */
    void on_savePlaylistButton_clicked();
    /**
     * load playlist dialog pops up
     *
     * @author Joel Stewart
     */
    void on_loadPlaylistButton_clicked();
    /**
     * clears playlist
     *
     * @author Joel Stewart
     */
    void on_clearPlaylistButton_clicked();
    /**
     * clears local library
     *
     * @author Joel Stewart
     */
    void on_clearLocalButton_clicked();
    /**
     * clears remote library
     *
     * @author Joel Stewart
     */
    void on_clearRemoteButton_clicked();
    /**
     * removes song
     *
     * @author Joel Stewart
     */
    void on_removeButton_clicked();
    /**
     * playlist item changed
     *
     * @param currentrow, current row of playlist
     *
     * @author Joel Stewart
     */
    void on_playlistWidget_currentRowChanged(int currentRow);
    /**
     * adds a folder of music to local library
     *
     * @author Joel Stewart
     */
    void on_action_Folder_triggered();
    /**
     * add an individual song to local library
     *
     * @author Joel Stewart
     */
    void on_action_Song_triggered();
    /**
     * tiger theme
     *
     * @author Joel Stewart
     */
    void on_action_Tiger_triggered();
    /**
     * default theme
     *
     * @author Joel Stewart
     */
    void on_action_Default_triggered();
    /**
     * download started
     *
     * @param filesize, size of file
     * @param packsizeRecv, size of packet
     * @param file, name of file
     *
     * @author Joel Stewart
     */
    void downloadStarted(int filesize, int packsizeRecv, QString file);
    /**
     * download is full
     *
     * @param full, is download queue full
     *
     * @author Joel Stewart
     */
    void downloadQueueFull(bool full);
    /**
     * host multicast dialog pops up
     *
     * @author Joel Stewart
     */
    void on_action_Host_Multicast_triggered();
};

#endif // MAINWINDOW_H
