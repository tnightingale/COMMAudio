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

void AudioComponent::startMic(QIODevice* stream, QThread* socketThread) {
    micIO_ = stream;
    format.setFrequency(44100);
    format.setChannels(2);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QThread* micThread = new QThread();
    input_ = new QAudioInput(format, NULL);
    connect(input_, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(mic(QAudio::State)));
    input_->moveToThread(micThread);
    micThread->start();
    input_->start(stream);
}

void AudioComponent::stopMic(){
    input_->stop();
    input_->deleteLater();
    //delete input_;
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

void AudioComponent::playStream(QIODevice* stream, QThread* socketThread){
    speakersIO_ = stream;
    format.setFrequency(44100);
    format.setChannels(2);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QThread* streamThread = new QThread();
    output_ = new QAudioOutput(format,NULL);
    connect(output_, SIGNAL(stateChanged(QAudio::State)),
            this, SLOT(speak(QAudio::State)));
    output_->moveToThread(streamThread);
    streamThread->start();
    output_->start(stream);
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
    if(allBuffers_.empty()){
        allBuffers_.append(new QBuffer);
        allFormats_.append(tempformat);
        output_= new QAudioOutput(allFormats_.first());
        output_->start(allBuffers_.first());
    }
    if(tempformat!=allFormats_.last()){
        allFormats_.append(tempformat);
        allBuffers_.append(new QBuffer);
        //new audio format append to format list
    }

    newData.remove(0,44);
    allBuffers_.last()->open(QIODevice::ReadOnly);
    allBuffers_.last()->write(newData);
    allBuffers_.last()->close();


}
/*
 * creates the AudioOutput
 *
 */
void AudioComponent::joinMulticast(){
    allBuffers_.clear();;
    allFormats_.clear();;
    //output_ = new QAudioOutput(format,0);


    //buff = output_->start();

    connect(output_,SIGNAL(stateChanged(QAudio::State)),this,SLOT(addToOutput(QAudio::State)));

}

void AudioComponent::writeToMulticast(QString fileName, QIODevice* socket){

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    //QByteArray* tempdata = &data; //used to view data....

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


    QByteArray header = data.remove(0,44);
    QDataStream dstream(&data,QIODevice::ReadOnly);

    while(!dstream.atEnd()){
        QByteArray newdata;
        char* chard =(char*) malloc(1024*8);

        dstream.readRawData(chard, 1024*8);
        newdata.append(chard,1024*8);
        //allBuffers_.append(newdata);
        socket->write(newdata.prepend(header));
    }

    file.close();

    buff = output_->start();
    while(output_->bytesFree()>1024*8){
        if(!allBuffers_.empty()){

            //buff->write(allBuffers_.takeFirst());
        }
        else{
            break;
        }
    }
    output_->setNotifyInterval(100);
    connect(output_,SIGNAL(notify()),this,SLOT(checkBuff()));
}

void AudioComponent::mic(QAudio::State newState){
    int error = 0;
    switch (newState) {
    case QAudio::StoppedState:
        if (input_->error() != QAudio::NoError) {
            // Perform error handling
            qDebug("mic error");
        } else {
            // Normal stop
        }
        break;

    case QAudio::SuspendedState:
        qDebug("mic suspended");

        break;
    case QAudio::ActiveState:
        qDebug("mic active");
        //
        break;
    case QAudio::IdleState:
        qDebug("mic idle");
        if ((error = input_->error()) != QAudio::NoError) {
            // Perform error handling
            qDebug("mic error: %d", error);
            if (error == QAudio::UnderrunError)
            {
                input_->start(micIO_);
            }
        }
        break;
    }
}

void AudioComponent::speak(QAudio::State newState){
    int error = 0;
    switch (newState) {
    case QAudio::StoppedState:
        if (output_->error() != QAudio::NoError) {
            // Perform error handling
            qDebug("speak error");
        } else {
            // Normal stop
        }
        break;

    case QAudio::SuspendedState:
        qDebug("speak suspended");

        break;
    case QAudio::ActiveState:
        qDebug("speak active");
        //
        break;
    case QAudio::IdleState:
        qDebug("speak idle");
        if ((error = output_->error()) != QAudio::NoError) {
            // Perform error handling
            qDebug("speak error: %d", error);
            if (error == QAudio::UnderrunError)
            {
                output_->start(speakersIO_);
            }
        }
        break;
    }
}
