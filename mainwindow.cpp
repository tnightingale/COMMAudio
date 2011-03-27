


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
/*
    Phonon::SeekSlider *slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(player_->getPlaylist());
    slider->setGeometry(180,490,450,19);
    slider->saveGeometry();
    slider->show();

*/


    ui->tab->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 60);"));

   // player->startMic();
    ui->remoteListWidget->setSortingEnabled(true);
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

void MainWindow::on_action_Join_Multicast_triggered()
{
    if (joinServer_.exec() == QDialog::Accepted)
    {

    }
}

void MainWindow::appendToRemote(QStringList songList, QString ipAddress)
{
    QString fileName, songTitle;
    for (int i = 0; i < songList.size();++i){
       fileName = songList.at(i);
       int n = fileName.lastIndexOf('/');
       int s = fileName.size() - n - 1;
       songTitle = fileName.right(s);
       remoteList_.insert(songTitle,*new RemoteSong(songTitle, ipAddress));
       ui->remoteListWidget->addItem(new QListWidgetItem(songTitle));
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
void MainWindow::on_remoteListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    RemoteSong songInfo = remoteList_.value(item->text());
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

    if(ui->playButton->text() == "Pause") {
       ui->playButton->setText("Play");
       player_->pause();
    } else {
       ui->playButton->setText("Pause");
       player_->play();

       QGraphicsItem *ball = new QGraphicsEllipseItem(0, 0, 20, 20);

       QTimeLine *timer = new QTimeLine(5000);
       timer->setFrameRange(0, 100);

       QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
       animation->setItem(ball);
       animation->setTimeLine(timer);

       for (int i = 0; i < 200; ++i)
            animation->setPosAt(i / 200.0, QPointF(i, i));

       QGraphicsScene *scene = new QGraphicsScene();
       scene->setSceneRect(0, 0, 250, 250);
       scene->addItem(ball);

       ui->visualGraphicsView->setScene(scene);
       ui->visualGraphicsView->show();

       timer->start();

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


bool MainWindow::winEvent(PMSG msg, long * result) {
    switch (msg->message) {
        case WM_WSAASYNC_TCP:
            emit signalWMWSASyncTCPRx(msg->wParam, msg->lParam);
            return true;

        case WM_WSAASYNC_UDP:
            emit signalWMWSASyncUDPRx(msg->wParam, msg->lParam);
            return true;
    }

    return false;
}

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
QStringList MainWindow::getLocalFileList()
{
    return player_->getFileList();
}

