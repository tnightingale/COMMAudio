#include "audiocomponent.h"
#include <QFileDialog>
#include <QTimer>
#include <QByteArray>
#include <QDirIterator>
#include <al.h>
#include <alc.h>
#include <QAudioDeviceInfo>
#include <QUrl>



AudioComponent::AudioComponent(QObject *parent) :
    QObject(parent)
{
    player_ = new QMediaPlayer;
    playlist_ = new QMediaPlaylist;
    player_->setPlaylist(playlist_);
    //playlist_ = new Phonon::MediaObject(this);
    //output_ = new Phonon::AudioOutput(Phonon::MusicCategory,this);
    //Phonon::createPath(playlist_,output_);
    //playlist_->setTransitionTime(-100);


}

void AudioComponent::setSourceFolder(){
    QString Folder = QFileDialog::getExistingDirectory(0,"Source Directory",".",QFileDialog::ShowDirsOnly);
    sourceFolder_ = QDir(Folder);

}
QStringList AudioComponent::getFileList(){
    QStringList stuff;
    QStringList filters;
    filters<< "*.wav" << "*.mp3";
    sourceFolder_.setNameFilters(filters);
    QDirIterator it(sourceFolder_, QDirIterator::Subdirectories);
    while (it.hasNext()) {
         stuff += it.next();//.entryInfoList(filters,QDir::Files,QDir::Name);
    }

    return stuff;
}

QMediaPlayer::State AudioComponent::getState(){
    return player_->state();
}

QList<QMediaContent> AudioComponent::getQueue() {
    QList<QMediaContent> temp;
    int count = playlist_->mediaCount();
    for (int i = 0 ; i < count; i++) {
        temp.append(playlist_->media(i));
    }
    return temp;
// return playlist_->queue();
}

void AudioComponent::addSongToBegining(QString filename) {
    addSong(filename);
    //QList<Phonon::MediaSource> queue = playlist_->queue();
}

void AudioComponent::setCurrentSong(QString fileName){
    playlist_->insertMedia(playlist_->nextIndex(),QUrl::fromLocalFile(fileName));
    playlist_->next();
    //playlist_->setCurrentSource(fileName);
}

void AudioComponent::addSong(QString filename) {

    playlist_->addMedia(QUrl::fromLocalFile(filename));
}
void AudioComponent::play() {
    player_->play();
}
void AudioComponent::pause() {
    player_->pause();
}
void AudioComponent::stop() {
    player_->stop();
}
void AudioComponent::next() {
    playlist_->next();
}
void AudioComponent::previous(){
    playlist_->previous();
}


void AudioComponent::startMic(){



    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(format));{
        qWarning()<<"format not supported";
        format = info.nearestFormat(format);
    }
    QStringList formatTypes = info.supportedCodecs();
    for(int i = 0;i < formatTypes.size();++i){
        qDebug()<< formatTypes.at(i);
    }

    input_ = new QAudioInput(format,NULL);
    QIODevice* micData = input_->start();
    QAudioOutput* qoutput_;
    qoutput_ = new QAudioOutput(format,NULL);
    qoutput_->start(micData);
    //return micData;
/*

    QTimer::singleShot(100, this, SLOT(stopMic()));

    buf = new QByteArray();
    inputBuffer_ = new QBuffer(buf,NULL);
    if(!inputBuffer_->open(QIODevice::ReadWrite)){
        qWarning("unable to open buffer");
    }

    input_->start(inputBuffer_);*/
}
void AudioComponent::stopMic(){

}
