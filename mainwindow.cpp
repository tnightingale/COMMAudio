#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audiocomponent.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    AudioComponent* player = new AudioComponent(this);
    /*player->setSourceFolder();

    player->addSong(player->getFileList().first().filePath());
*/

    //working player code for wav files. will play following 3 files from internet in succession


    /*player->addSong(QString("http://www.dailywav.com/0311/heavenOnEarth.wav"));
    player->addSong("http://www.dailywav.com/0311/spiteMe.wav");
    player->addSong("http://www.dailywav.com/0311/convictionsBourbon.wav");*/
    player->play();
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
