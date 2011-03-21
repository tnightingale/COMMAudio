#include "audiocomponent.h"
#include <QFileDialog>
#include <QTimer>
#include <QByteArray>
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
    allBuffers_.append(inputBuffer_);
    input_->start(inputBuffer_);
}
void AudioComponent::stopMic(){
    QAudioOutput* output;
    /*input_->stop();
    //outputFile.close();
    delete input_;
    qWarning()<<"done recording";
    startMic();
    //outputFile.setFileName("./test.wav");
    //outputFile.open(QIODevice::ReadOnly);*/
    //if(allBuffers_.size()>10){
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
    //}
}
