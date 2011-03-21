#include "mainwindow.h"
#include "ui_mainwindow.h"
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

   // player->startMic();

    for (int i = 0; i < player_->getFileList().size();++i){

       fileName = player_->getFileList().at(i);
       player_->addSong(fileName);
       int n = fileName.lastIndexOf('/');
       int s = fileName.size() - n - 1;
       songTitle = fileName.right(s);
       ui->clientListWidget->addItem(new QListWidgetItem(songTitle));

    }
    if(player_->getFileList().size() > 0) {
        fileName = player_->getFileList().at(0);
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
    QList<Phonon::MediaSource> queue = player_->getQueue();
    QString dataClicked = item->text();
    QString fullPath = findFullPath(dataClicked);
    player_->setCurrentSong(fullPath);
    ui->currentSongEditBox->setText(dataClicked);
    qDebug(qPrintable(item->text()));
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
    player_->play();
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
    player_->pause();
}

