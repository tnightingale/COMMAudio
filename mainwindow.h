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
    bool winEvent(MSG * msg, long * result);

    /**
     * adds to the QMap
     *
     * @param songList, the song list recieved
     * @param ipAddress, ip address where fileList was revieved from
     *
     * @author Joel Stewart
     */
    void appendToRemote(QStringList songList, QString ipAddress);

    QRectF rect (int r);

    void setupRot (QTimeLine *timeline, QGraphicsItem *item);

    QStringList getLocalFileList();

signals:
    void signalWMWSASyncTCPRx(PMSG);
    void signalWMWSASyncUDPRx(PMSG);

private:
    Ui::MainWindow *ui;
    TCPSocket *controlSocket_;
    AudioComponent* player_;
    JoinServer joinServer_;
    QMap<QString,RemoteSong> remoteList_;

private slots:
    void on_action_Join_Multicast_triggered();
    void on_action_Visible_toggled(bool status);
    void on_clientListWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_playButton_clicked();
    void on_stopButton_clicked();
    void on_remoteListWidget_itemDoubleClicked(QListWidgetItem* item);
};

#endif // MAINWINDOW_H
