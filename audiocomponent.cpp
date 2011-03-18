#include "audiocomponent.h"
#include <QFileDialog>
AudioComponent::AudioComponent(QObject *parent) :
    QObject(parent)
{
    playlist_ = new Phonon::MediaObject(this);
    output_ = new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(playlist_,output_);
}

void AudioComponent::setSourceFolder(){
    QString Folder = QFileDialog::getExistingDirectory(0,"Source Directory",".",QFileDialog::ShowDirsOnly);
    sourceFolder_ = QDir(Folder);
    /*
    QFileDialog dialog(this,NULL);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(dialog.exec()){
        fileNames = dialog.selectedFiles();
    }*/
}
QFileInfoList AudioComponent::getFileList(){
    QStringList filters;
    filters<< "*.wav" << "*.mp3";
    return sourceFolder_.entryInfoList(filters,QDir::Files,QDir::Name);

}

void AudioComponent::addSong(QString filename) {

    playlist_->enqueue(filename);
}
void AudioComponent::play() {
    playlist_->play();
}
void AudioComponent::pause() {
    playlist_->pause();
}
void AudioComponent::stop() {
    playlist_->stop();
}
