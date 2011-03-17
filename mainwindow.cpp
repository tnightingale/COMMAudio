#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiocomponent.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    /*
    AudioComponent* player = new AudioComponent(this);
     //working player code for wav files. will play following 3 files from internet in succession
    player->addSong(QString("http://www.dailywav.com/0311/heavenOnEarth.wav"));
    player->addSong("http://www.dailywav.com/0311/spiteMe.wav");
    player->addSong("http://www.dailywav.com/0311/convictionsBourbon.wav");
    player->play();*/
}

MainWindow::~MainWindow()
{
    delete ui;
}
