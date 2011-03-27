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

    timer_ = new QTimeLine(50000);
    visualization(40);
    timer_->setPaused(true);
    ui->tab->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);color: white;"));
    ui->tab_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);color: white;"));
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

void MainWindow::on_action_Request_Playlist_triggered()
{
    if (requestPlaylist_.exec() == QDialog::Accepted)
    {

    }
}

void MainWindow::visualization(int n) {
    QGraphicsRectItem *rect = new QGraphicsRectItem(0,0,80,n);
    QGraphicsRectItem *rect2 = new QGraphicsRectItem(85,0,80,n);
    QGraphicsRectItem *rect3 = new QGraphicsRectItem(170,0,80,n);
    QGraphicsRectItem *rect4 = new QGraphicsRectItem(255,0,80,n);
    QGraphicsRectItem *rect5 = new QGraphicsRectItem(340,0,80,n);
    QGraphicsRectItem *rect6 = new QGraphicsRectItem(425,0,80,n);
    QGraphicsRectItem *rect7 = new QGraphicsRectItem(510,0,80,n);
    QGraphicsRectItem *rect8 = new QGraphicsRectItem(595,0,80,n);
    QGraphicsRectItem *rect9 = new QGraphicsRectItem(680,0,80,n);

    QBrush fillBrush(Qt::red);
    rect->setBrush(fillBrush);
    rect2->setBrush(fillBrush);
    rect3->setBrush(fillBrush);
    rect4->setBrush(fillBrush);
    rect5->setBrush(fillBrush);
    rect6->setBrush(fillBrush);
    rect7->setBrush(fillBrush);
    rect8->setBrush(fillBrush);
    rect9->setBrush(fillBrush);

    timer_->setCurveShape(QTimeLine::SineCurve);
    timer_->setLoopCount(0);
    timer_->setFrameRange(0, 100);

    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation2 = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation3 = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation4 = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation5 = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation6 = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation7 = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation8 = new QGraphicsItemAnimation;
    QGraphicsItemAnimation *animation9 = new QGraphicsItemAnimation;

    animation->setItem(rect);
    animation->setTimeLine(timer_);

    animation2->setItem(rect2);
    animation2->setTimeLine(timer_);

    animation3->setItem(rect3);
    animation3->setTimeLine(timer_);

    animation4->setItem(rect4);
    animation4->setTimeLine(timer_);

    animation5->setItem(rect5);
    animation5->setTimeLine(timer_);

    animation6->setItem(rect6);
    animation6->setTimeLine(timer_);

    animation7->setItem(rect7);
    animation7->setTimeLine(timer_);

    animation8->setItem(rect8);
    animation8->setTimeLine(timer_);

    animation9->setItem(rect9);
    animation9->setTimeLine(timer_);

    for (int i = 0; i < 100.0; ++i) {
        animation->setScaleAt(i/100.0,1,i%9);
        animation2->setScaleAt(i/100.0,1,i%7);
        animation3->setScaleAt(i/100.0,1,i%5);
        animation4->setScaleAt(i/100.0,1,i%4);
        animation5->setScaleAt(i/100.0,1,i%9);
        animation6->setScaleAt(i/100.0,1,i%7);
        animation7->setScaleAt(i/100.0,1,i%5);
        animation8->setScaleAt(i/100.0,1,i%7);
        animation9->setScaleAt(i/100.0,1,i%9);
    }

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 840, 410);
    scene->addItem(rect);
    scene->addItem(rect2);
    scene->addItem(rect3);
    scene->addItem(rect4);
    scene->addItem(rect5);
    scene->addItem(rect6);
    scene->addItem(rect7);
    scene->addItem(rect8);
    scene->addItem(rect9);

    ui->visualGraphicsView->setScene(scene);
    ui->visualGraphicsView->show();

    timer_->start();
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
    ui->playButton->setText("Pause");
    timer_->setPaused(true);
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
       timer_->setPaused(true);
    } else {
       ui->playButton->setText("Pause");
       player_->play();
       if(ui->tabWidget->currentIndex() == 1) {
            timer_->setPaused(false);
       } else {
           timer_->setPaused(true);
       }
    }s

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
    ui->playButton->setText("Play");
    timer_->setPaused(true);
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


void MainWindow::on_talkButton_pressed()
{
    player_->startMic();
}

void MainWindow::on_talkButton_released()
{
    player_->stopMic();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(ui->playButton->text() == "Pause") {
        if(index == 0) {
            timer_->setPaused(true);
        } else {
            timer_->setPaused(false);
        }
    } else {
        timer_->setPaused(true);
    }
    qDebug("%d", index);
}
