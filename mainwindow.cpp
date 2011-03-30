#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiocomponent.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    slider_(0),
    muted_(false)
{
    ui->setupUi(this);
    ui->currentSongEditBox->setReadOnly(true);

    player_ = new AudioComponent(this);
    /*player->addSong("./test.raw");
    player->play();*/

    /*
    Phonon::SeekSlider *slider = new Phonon::SeekSlider(this);
    slider->setMediaObject(player_->getPlaylist());
    slider->setGeometry(180,490,450,19);
    slider->saveGeometry();
    slider->show();
*/
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    ui->previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    ui->muteToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    playerlink_ = player_->getPlayer();
    connect(playerlink_, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(playerlink_, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    slider_ = new QSlider(Qt::Horizontal, this);
    slider_->setRange(0, playerlink_->duration() / 1000);
    slider_->setGeometry(180,485,450,19);
    slider_->saveGeometry();
    connect(slider_, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    timer_ = new QTimeLine(50000);
    visualization(40);
    timer_->setPaused(true);

    backgroundColor("black", "White");
   // player->startMic();

    ui->remoteListWidget->setSortingEnabled(true);
   // ui->clientListWidget->setSortingEnabled(true);
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
    for (int i = 0; i < songList_.size();++i){
        fullPath = songList_.at(i);
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
        emit requestPlaylist(requestPlaylist_.getIp(),
                             requestPlaylist_.getPort());
    }
}

void MainWindow::seek(int seconds)
{
    playerlink_->setPosition(seconds * 1000);
}

void MainWindow::durationChanged(qint64 duration)
{
    slider_->setMaximum(duration / 1000);
}

void MainWindow::positionChanged(qint64 progress)
{
    if (!slider_->isSliderDown()) {
        slider_->setValue(progress / 1000);
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

void MainWindow::appendToRemote(QStringList songList_, QString ipAddress)
{
    QString fileName, songTitle;
    for (int i = 0; i < songList_.size();++i){
        fileName = songList_.at(i);
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
    QString dataClicked = item->text();
    QString fullPath = findFullPath(dataClicked);
    if(player_->addSong(fullPath)) {
        ui->playlistWidget->addItem(new QListWidgetItem(dataClicked));
        ui->songAddedEditBox->setText(dataClicked);
        if(ui->playlistWidget->count() == 1) {
            ui->currentSongEditBox->setText(dataClicked);
        }
    }
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
    emit requestFile(songInfo.getIp(), songInfo.getFilePath());
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
       ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
       player_->pause();
       timer_->setPaused(true);
    } else {
       ui->playButton->setText("Pause");
       ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
       player_->play();

       if(ui->tabWidget->currentIndex() == 1) {
            timer_->setPaused(false);
       } else {
           timer_->setPaused(true);
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
    return songList_;
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
}

void MainWindow::on_nextButton_clicked()
{
    int max = (ui->playlistWidget->count() - 1);
    int current = ui->playlistWidget->currentIndex().row();
    if(current == max) {
        player_->gotoIndex(-1);
        ui->playlistWidget->setCurrentRow(0);
        ui->currentSongEditBox->setText(ui->playlistWidget->currentItem()->text());
    } else {
        player_->next();
        QString song = playerlink_->media().canonicalUrl().toString();
        int n = song.lastIndexOf('/');
        int s = song.size() - n - 1;
        QString songTitle = song.right(s);
        ui->currentSongEditBox->setText(songTitle);
        ui->playlistWidget->setCurrentRow(ui->playlistWidget->currentRow() + 1);
    }
}

void MainWindow::on_previousButton_clicked()
{
    int max = (ui->playlistWidget->count() - 1);
    int current = ui->playlistWidget->currentIndex().row();
    if(current == 0) {
        player_->gotoIndex(max);
        ui->playlistWidget->setCurrentRow(max);
        ui->currentSongEditBox->setText(ui->playlistWidget->currentItem()->text());
    } else {
        player_->previous();
        QString song = playerlink_->media().canonicalUrl().toString();
        int n = song.lastIndexOf('/');
        int s = song.size() - n - 1;
        QString songTitle = song.right(s);
        ui->currentSongEditBox->setText(songTitle);
        ui->playlistWidget->setCurrentRow(ui->playlistWidget->currentRow() - 1);
    }
}

void MainWindow::on_muteToolButton_clicked()
{
    if(muted_) {
        ui->muteToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        muted_ = false;
        playerlink_->setMuted(false);
    } else {
        muted_ = true;
        playerlink_->setMuted(true);
        ui->muteToolButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }

}

void MainWindow::on_horizontalSlider_valueChanged(int volume)
{
    playerlink_->setVolume(volume);
    ui->volumeLcdNumber->display(volume);
}

void MainWindow::backgroundColor(QString background, QString font) {
    QString backColor = background, fontColor = font;
    const QString maincolor = "* {color:" + background + ";background-color:" + font + ";}";
    font.prepend(";color:");
    font.append(";}");
    QString hover = background;
    hover.prepend("QPushButton {border-style:ridge;border-width:2px;border-color:red;background-color:");
    hover.append(font);
    QString sliderColor = "QSlider::groove:horizontal {background:" +  fontColor + ";position: absolute;"
                            "border-style:solid;border-width:3px;border-color:" + backColor + ";}";
    sliderColor += "QSlider::handle:horizontal {height: 20px;background:" + backColor + ";width:4px;"
                    "border-style:solid;border-width:1px;border-color:" + fontColor + ";}";
    sliderColor += "QSlider::add-page:horizontal {background:" + fontColor + ";}";
    sliderColor += "QSlider::sub-page:horizontal {background:" + backColor + ";}";
    sliderColor += "QSlider {border-style:groove;border-width:2px;border-color:" + backColor + ";}";

    const QString sliderMods = sliderColor;
    background = background.prepend("QWidget, QLCDNumber, * {background-color:");
    background = background.append(font);
    const QString color = background;
    const QString button = hover;
    const QString tabColor = "QTabWidget::tab-bar {background: Red;} QTabBar::tab"
                            "{background:" + backColor + ";color:" + fontColor + ";}" + color +
                            "QTabBar::tab:hover{background:" + fontColor + ";color:" + backColor + ";}";
    ui->tabWidget->setStyleSheet(tabColor);
    ui->volumeLcdNumber->setStyleSheet(color);
    ui->playbackBox->setStyleSheet(color);
    ui->playButton->setStyleSheet(button);
    ui->stopButton->setStyleSheet(button);
    ui->previousButton->setStyleSheet(button);
    ui->talkButton->setStyleSheet(button);
    ui->muteToolButton->setStyleSheet(button);
    ui->nextButton->setStyleSheet(button);
    ui->addMusicButton->setStyleSheet(button);
    slider_->setStyleSheet(sliderMods);
    ui->horizontalSlider->setStyleSheet(sliderMods);
    MainWindow::setStyleSheet(maincolor);
    ui->menuBar->setStyleSheet(color + "QMenuBar::item {background:" + backColor + ";color:" + fontColor + ";}"
                               "QMenuBar::item:selected {background:" + fontColor + ";color:" + backColor + ";}"
                               "QMenu::item:selected {background:" + fontColor + ";color:" + backColor + ";}");
}

void MainWindow::on_action_Modify_triggered(){
    if (changeColor_.exec() == QDialog::Accepted)
    {
        backgroundColor(changeColor_.getBackground(), changeColor_.getFont());
    }
}

void MainWindow::on_playlistWidget_itemDoubleClicked(QListWidgetItem* item)
{
    ui->currentSongEditBox->setText(item->text());
    ui->playlistWidget->setCurrentItem(item);
    int selected = ui->playlistWidget->currentRow();
    if (selected == 0) {
        player_->gotoIndex(-1);
    } else {
        player_->gotoIndex(selected);
    }
}

void MainWindow::on_addMusicButton_clicked()
{

    QString fileName;
    QString songTitle;

    player_->setSourceFolder();
    QStringList songs = player_->getFileList();
    for (int i = 0; i < songs.size();++i){
        fileName = songs.at(i);
        int n = fileName.lastIndexOf('/');
        int s = fileName.size() - n - 1;
        songTitle = fileName.right(s);
        ui->clientListWidget->addItem(new QListWidgetItem(songTitle));

    }
    songList_ += songs;
}

void MainWindow::on_playbackBox_valueChanged(double playback)
{
    playerlink_->setPlaybackRate(playback);
}
