#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    player_ = new AudioComponent(this);
    /*player->addSong("./test.raw");
    player->play();*/
    player_->setSourceFolder();

   // player->startMic();

    for (int i = 0; i < player_->getFileList().size();++i){

       player_->addSong(player_->getFileList().at(i));
       ui->clientListWidget->addItem(new QListWidgetItem(player_->getFileList().at(i)));

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


void MainWindow::on_clientListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    QList<Phonon::MediaSource> queue = player_->getQueue();
    QString dataClicked = item->text();
    qDebug(qPrintable(item->text()));
}
