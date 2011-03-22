#include "audiocomponent.h"
#include <QFileDialog>
#include <QTimer>
#include <QByteArray>
#include <QDirIterator>
AudioComponent::AudioComponent(QObject *parent) :
    QObject(parent)
{
    playlist_ = new Phonon::MediaObject(this);
    output_ = new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(playlist_,output_);
    playlist_->setTransitionTime(-100);


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

Phonon::State AudioComponent::getState(){
    return playlist_->state();
}

QList<Phonon::MediaSource> AudioComponent::getQueue() {
    return playlist_->queue();
}

void AudioComponent::addSongToBegining(QString filename) {
    QList<Phonon::MediaSource> queue = playlist_->queue();
}

void AudioComponent::setCurrentSong(QString fileName){
    playlist_->setCurrentSource(fileName);
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


void AudioComponent::startMic(){
    QByteArray* buf;

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

    input_ = new QAudioInput(format,NULL);
    QTimer::singleShot(100, this, SLOT(stopMic()));
/*
    outputFile.setFileName("./test.wav");
    outputFile.open(QIODevice::WriteOnly|QIODevice::Truncate);*/
    buf = new QByteArray();
    inputBuffer_ = new QBuffer(buf,NULL);
    if(!inputBuffer_->open(QIODevice::ReadWrite)){
        qWarning("unable to open buffer");
    }

    input_->start(inputBuffer_);
}
void AudioComponent::stopMic(){
    //QAudioOutput* output;

    //allBuffers_.append(inputBuffer_);
    input_->stop();
    //outputFile.close();
   // delete input_;
    qWarning()<<"done recording";
    //startMic();
    //outputFile.setFileName("./test.wav");
    //outputFile.open(QIODevice::ReadOnly);


    playlist_->enqueue(inputBuffer_);
    delete input_;
    input_ = new QAudioInput(format,NULL);
    inputBuffer_ = new QBuffer(new QByteArray());

    input_->start(inputBuffer_);
    playlist_->play();
   /* if(allBuffers_.size()>10){
        QAudioDeviceInfo info( QAudioDeviceInfo::defaultOutputDevice());
        if (!info.isFormatSupported(format));{
            qWarning()<<"format not supported";
            format = info.nearestFormat(format);
        }
        output = new QAudioOutput(format, NULL);
        buffer_ = new QBuffer();
        buffer_->setBuffer(&allBuffers_.takeFirst()->buffer());
        if(!buffer_->open(QIODevice::ReadWrite)){
            qWarning("unable to open buffer");
        }
        buffer_->seek(0);
        output->start(buffer_);
    }*/
}
