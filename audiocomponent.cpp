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
    filters << "*.wav" << "*.mp3";
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

}

bool AudioComponent::addSongToBegining(QString filename) {
    return addSong(filename);

}

void AudioComponent::setCurrentSong(QString fileName){
    playlist_->insertMedia(playlist_->nextIndex(),QUrl::fromLocalFile(fileName));
    playlist_->next();

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

void AudioComponent::startMic(QIODevice* stream, QThread* micThread) {
    micIO_ = stream;
    format.setFrequency(44100);
    format.setChannels(2);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    input_ = new QAudioInput(format, NULL);
    connect(input_, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(mic(QAudio::State)));
    input_->moveToThread(micThread);
    input_->start(stream);
    isMicOn = true;
}

void AudioComponent::stopMic(){
    input_->stop();
    disconnect(input_, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(mic(QAudio::State)));
    isMicOn = false;
    input_->deleteLater();

}

void AudioComponent::pauseMic()
{
    if (input_->state() == QAudio::ActiveState)
    {
        input_->suspend();
    }
}

void AudioComponent::resumeMic()
{
    if (input_->state() == QAudio::SuspendedState)
    {
        input_->resume();
    }
}

void AudioComponent::playStream(QIODevice* stream, QThread* streamThread){
    speakersIO_ = stream;
    format.setFrequency(44100);
    format.setChannels(2);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    output_ = new QAudioOutput(format,NULL);
    connect(output_, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(speak(QAudio::State)));
    output_->moveToThread(streamThread);
    output_->start(stream);
    isSpeakersOn = true;
}

void AudioComponent::stopStream()
{
    output_->stop();
    disconnect(output_, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(speak(QAudio::State)));
    isSpeakersOn = false;
    output_->deleteLater();
}

void AudioComponent::addFromMulticast(Socket* socket) {

    QByteArray newData = socket->read(1024*8+44);

    QAudioFormat tempformat;
    int position = 20;
    int temp = *(short*)&newData.constData()[position];
    if(temp!=1){
        return;
    }
    position = 24;
    temp = *(int*)&newData.constData()[position];
    tempformat.setSampleRate(temp);
    position = 22;
    temp = *(short*)&newData.constData()[position];
    tempformat.setChannels(temp);
    position = 34;
    temp = *(short*)&newData.constData()[position];
    tempformat.setSampleSize(temp);
    tempformat.setCodec("audio/pcm");
    tempformat.setByteOrder(QAudioFormat::LittleEndian);
    if(tempformat.sampleSize()==8){
        tempformat.setSampleType(QAudioFormat::UnSignedInt);
    }else{
        tempformat.setSampleType(QAudioFormat::SignedInt);
    }
    if(allFormats_.isEmpty()){
        allBuffers_.append(new QList<QByteArray*>);
        allFormats_.append(tempformat);
        output_= new QAudioOutput(allFormats_.first(),NULL);
        connect(output_,SIGNAL(stateChanged(QAudio::State)),this,SLOT(stateChangeStream(QAudio::State)));
        output_->setBufferSize(1024*8*10);
        buff = output_->start();


    }
    newData.remove(0,44);
    if(!(tempformat==allFormats_.last())){
        allBuffers_.append(new QList<QByteArray*>);
        allFormats_.append(tempformat);

        //new audio format append to format list
    }
    allBuffers_.last()->append(&newData);

    int i;

    while((i = output_->bytesFree())>1024*8 ) {
        if(!allBuffers_.first()->isEmpty()){
            QByteArray temparrrrrr = *allBuffers_.first()->takeFirst();
            buff->write(temparrrrrr);
        }
        else {
            break;
        }
    }
}

void AudioComponent::joinMulticast(){
    allBuffers_.clear();;
    allFormats_.clear();;
}

void AudioComponent::stateChangeStream(QAudio::State newState){
    switch (newState) {
    case QAudio::SuspendedState:
        qDebug("AudioComponent::stateChangeStream(); SuspendedState.");
        break;

    case QAudio::ActiveState:
        qDebug("AudioComponent::stateChangeStream(); ActiveState.");
        break;

    case QAudio::IdleState:
        qDebug("AudioComponent::stateChangeStream(); IdleState.");

        if(allFormats_.size()!=1){
            qDebug("AudioComponent::stateChangeStream(); Switching songs.");
            output_->stop();
            output_->deleteLater();

            allBuffers_.removeFirst();
            allFormats_.removeFirst();
            output_= new QAudioOutput(allFormats_.first());
            buff = output_->start();
        }
        break;
    default:
        break;
    }
}

void AudioComponent::mic(QAudio::State newState){
    if (!isMicOn)
    {
        return;
    }
    int error = 0;
    switch (newState) {
    case QAudio::SuspendedState:
        qDebug("AudioComponent::mic(); State: SuspendedState.");
        break;

    case QAudio::ActiveState:
        qDebug("AudioComponent::mic(); State: ActiveState.");
        break;

    case QAudio::IdleState:
        qDebug("AudioComponetn:mic(); State: IdleState.");
        if ((error = input_->error()) != QAudio::NoError) {
            if (error == QAudio::UnderrunError) {
                qDebug("AudioComponent::mic() Underrun error.");
                input_->start(micIO_);
            }
        }
        break;
    }
}

void AudioComponent::speak(QAudio::State newState){
    if (!isSpeakersOn)
    {
        return;
    }
    int error = 0;
    switch (newState) {
    case QAudio::SuspendedState:
        qDebug("AudioComponent::speak(); State: SuspendedState.");
        break;

    case QAudio::ActiveState:
        qDebug("AudioComponent::speak(); State: ActiveState.");
        break;

    case QAudio::IdleState:
        qDebug("AudioComponent::speak(); State: IdleState.");
        if ((error = output_->error()) != QAudio::NoError) {
            if (error == QAudio::UnderrunError) {
                qDebug("AudioComponent::speak(); Underrun error.");
                output_->start(speakersIO_);
            }
        }
        break;
    default:
        break;
    }
}
