#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDebug>
#include <Phonon/MediaSource>
#include <Phonon/SeekSlider>
#include "tcpsocket.h"
#include "audiocomponent.h"
#include "joinserver.h"
#include "ui_joinserver.h"

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

private:
    Ui::MainWindow *ui;
    TCPSocket *controlSocket_;
    AudioComponent* player_;
    JoinServer joinServer_;

private slots:
    void on_action_Join_Multicast_triggered();
    void on_action_Visible_toggled(bool status);
    void on_clientListWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_playButton_clicked();
    void on_stopButton_clicked();
    void on_pauseButton_clicked();
    void on_remoteListWidget_2_itemDoubleClicked(QListWidgetItem* item);
};

#endif // MAINWINDOW_H
