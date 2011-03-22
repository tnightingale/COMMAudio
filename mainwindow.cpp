#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiocomponent.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString fileName;
    QString songTitle;

    ui->setupUi(this);
    ui->currentSongEditBox->setReadOnly(true);

    player_ = new AudioComponent(this);
    /*player->addSong("./test.raw");
    player->play();*/
    player_->setSourceFolder();

    Phonon::SeekSlider *slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(player_->getPlaylist());
    slider->setGeometry(180,490,450,19);
    slider->saveGeometry();
    slider->show();

   // player->startMic();
    ui->remoteListWidget_2->setSortingEnabled(true);
    //ui->clientListWidget->setSortingEnabled(true);
    QStringList songList = player_->getFileList();
    for (int i = 0; i < songList.size();++i){

       fileName = songList.at(i);
       player_->addSong(fileName);
       int n = fileName.lastIndexOf('/');
       int s = fileName.size() - n - 1;
       songTitle = fileName.right(s);
       ui->clientListWidget->addItem(new QListWidgetItem(songTitle));

    }
    if(songList.size() > 0) {
        fileName = songList.at(0);
        int n = fileName.lastIndexOf('/');
        int s = fileName.size() - n - 1;
        songTitle = fileName.right(s);
        ui->currentSongEditBox->setText(songTitle);
    }
    //working player code for wav files. will play following 3 files from internet in succession


    /*player->addSong(QString("http://www.dailywav.com/0311/heavenOnEarth.wav"));
    player->addSong("http://www.dailywav.com/0311/spiteMe.wav");
    player->addSong("http://www.dailywav.com/0311/convictionsBourbon.wav");*/
    //player->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::findFullPath(QString filename) {
    QString fullPath;
    for (int i = 0; i < player_->getFileList().size();++i){
       fullPath = player_->getFileList().at(i);
       if(fullPath.contains(filename)) {
           return fullPath;
       }
    }
    return NULL;
}

void MainWindow::on_action_Visible_toggled(bool status)
{
    if (status)
    {
        // Create the listen socket
        return;
    }
    else
    {
        // Either close the socket here or find a way to hide it
        return;
    }
}

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
void MainWindow::on_clientListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    player_->stop();
    QString dataClicked = item->text();
    QString fullPath = findFullPath(dataClicked);
    player_->setCurrentSong(fullPath);
    ui->currentSongEditBox->setText(dataClicked);
    qDebug(qPrintable(item->text()));
    player_->play();
}

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
void MainWindow::on_remoteListWidget_2_itemDoubleClicked(QListWidgetItem* item)
{

}

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
void MainWindow::on_playButton_clicked()
{
    if(player_->getState() == Phonon::StoppedState ||
            player_->getState() == Phonon::PausedState) {
        player_->play();
    } else {
        switch(player_->getState()) {
        case Phonon::ErrorState:
            qDebug("Error");
            break;
        case Phonon::LoadingState:
            qDebug("Loading");
        }
    }
}

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
void MainWindow::on_stopButton_clicked()
{
    player_->stop();
}


/*
-- FUNCTION: on_pauseButton_clicked
--
-- DATE: March 21, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Joel Stewart
--
-- PROGRAMMER: Joel Stewart
--
-- INTERFACE: void MainWindow::on_pauseButton_clicked()
--
-- RETURNS:
--
-- NOTES:
-- Pauses song
*/
void MainWindow::on_pauseButton_clicked()
{
    if(player_->getState() == Phonon::PlayingState) {
        player_->pause();
    }
}

bool MainWindow::winEvent(MSG * msg, long * result) {
    switch (msg->message) {
        case WM_WSAASYNC_TCP:
            emit signalWMWSASyncTCPRx(msg);
            return true;

        case WM_WSAASYNC_UDP:
            emit signalWMWSASyncUDPRx(msg);
            return true;
    }

    return false;
}
