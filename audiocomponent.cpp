#include "audiocomponent.h"

AudioComponent::AudioComponent(QObject *parent) :
    QObject(parent)
{
    playlist_ = new Phonon::MediaObject(this);
    output_ = new Phonon::AudioOutput(Phonon::MusicCategory,this);
    Phonon::createPath(playlist_,output_);
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
