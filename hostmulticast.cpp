#include "hostmulticast.h"
#include "ui_hostmulticast.h"

HostMulticast::HostMulticast(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HostMulticast)
{
    ui->setupUi(this);
    QWidget::setWindowIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playlist_ = new QStringList();
    ui->playButton->setText("Play");
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    emit startMulticast(playlist_);
    this->setFixedSize(733,477);
}

HostMulticast::~HostMulticast()
{
    delete ui;
}

void HostMulticast::on_playButton_clicked()
{
    if(ui->playlistWidget->count() > 0) {
        if(ui->playButton->text() == "Pause") {
            ui->playButton->setText("Play");
            ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            emit pause();
        } else {
            ui->playButton->setText("Pause");
            ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            emit play();
        }
    }
}

void HostMulticast::on_localLibraryList_itemDoubleClicked(QListWidgetItem* item)
{
    ui->playlistWidget->addItem(new QListWidgetItem(item->text()));
    playlist_->append(findFullPath(item->text()));
}

QString HostMulticast::findFullPath(QString filename) {
    QString fullPath;
    for (int i = 0; i < library_.size();++i){
        fullPath = library_.at(i);
        if(fullPath.contains(filename)) {
            return fullPath;
        }
    }
    return NULL;
}
void HostMulticast::loadLibrary(QStringList* library) {
    ui->localLibraryList->clear();
    library_ = QStringList(*library);
    QString fileName, songTitle;
    for (int i = 0; i < library_.size();++i){
        fileName = library_.at(i);
        if(fileName.contains(".wav")) {
            int n = fileName.lastIndexOf('/');
            int s = fileName.size() - n - 1;
            songTitle = fileName.right(s);
            ui->localLibraryList->addItem(new QListWidgetItem(songTitle));
        }
    }
}
