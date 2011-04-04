#include "audiocomponent.h"
#include <QFileDialog>
#include <QTimer>
#include <QByteArray>
#include <QDirIterator>
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
    playlist_->currentIndex();
}

void AudioComponent::gotoIndex(int index) {
    playlist_->setCurrentIndex(index);
}

int AudioComponent::getIndex(){
    return playlist_->currentIndex();
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

bool AudioComponent::addSongToBegining(QString filename) {
    return addSong(filename);
    //QList<Phonon::MediaSource> queue = playlist_->queue();
}

void AudioComponent::setCurrentSong(QString fileName){
    playlist_->insertMedia(playlist_->nextIndex(),QUrl::fromLocalFile(fileName));
    playlist_->next();
    //playlist_->setCurrentSource(fileName);
}

bool AudioComponent::addSong(QString filename) {

   return playlist_->addMedia(QUrl::fromLocalFile(filename));
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

QMediaPlayer* AudioComponent::getPlayer() {
    return player_;
}

void AudioComponent::previousIndex(int desiredIndex) {
    while(playlist_->previousIndex(1)!= desiredIndex){
        playlist_->previous();
    }
}

void AudioComponent::nextIndex(int desiredIndex) {
    while(playlist_->nextIndex(1)!= desiredIndex){
        playlist_->next();
    }
}

QMediaPlaylist* AudioComponent::getPlaylist() {
    return playlist_;
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
    QAudioOutput* qoutput_;
    qoutput_ = new QAudioOutput(format,NULL);

    input_->start(qoutput_->start());



    //return micData;
}
void AudioComponent::startMic(QIODevice* stream) {
    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    input_ = new QAudioInput(format,NULL);
    input_->start(stream);
}

void AudioComponent::stopMic(){
    input_->stop();
    input_->deleteLater();
    //delete input_;
}

void AudioComponent::playStream(QIODevice* stream){
    format.setFrequency(8000);
    format.setChannels(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioOutput* qoutput_;
    qoutput_ = new QAudioOutput(format,NULL);
    qoutput_->start(stream);
}

void AudioComponent::testwav(QString fileName){

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    QByteArray* tempdata = &data; //used to view data....

    int position = 0;

    position = 16;
    position = 20;
    int temp = *(short*)&data.constData()[position];
    if(temp!=1){
        return;
    }


    position = 24;
     temp = *(int*)&data.constData()[position];
    format.setSampleRate(temp);
    position = 22;
    temp = *(short*)&data.constData()[position];
    format.setChannels(temp);
    position = 34;
    temp = *(short*)&data.constData()[position];
    format.setSampleSize(temp);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    if(format.sampleSize()==8){
        format.setSampleType(QAudioFormat::UnSignedInt);
    }else{
        format.setSampleType(QAudioFormat::SignedInt);
    }


    output_ = new QAudioOutput(format,0);
    //output_->setBufferSize(1024*8*10);

    data.remove(0,44);
/*    QDataStream dstream(&data,QIODevice::ReadOnly);

    while(!dstream.atEnd()){
        QByteArray newdata;
        char* chard =(char*) malloc(1024*8);

        dstream.readRawData(chard, 1024*8);
        newdata.append(chard,1024*8);
        allBuffers_.append(newdata);
    }*/

    file.close();
    buff = new QBuffer(&data,0);
    buff->open(QIODevice::ReadOnly);
    output_->start(buff);
/*
    buff = output_->start();
    while(output_->bytesFree()>1024*8){
        if(!allBuffers_.empty()){
            buff->write(allBuffers_.takeFirst());
        }
        else{
            break;
        }
    }*//*
    output_->setNotifyInterval(100);
    connect(output_,SIGNAL(notify()),this,SLOT(checkBuff()));*/
    //connect(output_,SIGNAL(stateChanged(QAudio::State)),this,SLOT(addToOutput(QAudio::State)));

}
void AudioComponent::checkBuff(){
   /* int i = output_->bytesFree();
    int j = output_->notifyInterval();
    while((i = output_->bytesFree()) > 1024*8){
        if(!allBuffers_.empty()){
            buff->write(allBuffers_.takeFirst());
        }
        else {
            break;
        }
    }*/
}

void AudioComponent::addToOutput(QAudio::State newState){
    switch (newState) {
        case QAudio::StoppedState:
            if (output_->error() != QAudio::NoError) {
             // Perform error handling
                qDebug("blahhhhhh error");
            } else {
             // Normal stop
            }
            break;

        case QAudio::SuspendedState:
            qDebug("blahhhhhh suspended");

            break;
            case QAudio::ActiveState:
            qDebug("blahhhhhh active");
            //
            break;
            case QAudio::IdleState:
            qDebug("blahhhhhh idle");
            buff->write(data);
            break;
    }


}




