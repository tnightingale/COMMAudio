#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <QObject>
#include <Phonon>
#include <QDir>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QtMultimediaKit/QMediaPlayer>
#include <QtMultimediaKit/QMediaPlaylist>
#include <QtMultimediaKit/QMediaObject>

class AudioComponent : public QObject
{
    Q_OBJECT
public:
    explicit AudioComponent(QObject *parent = 0);


    void addSong(QString filename);
    void setSourceFolder();
    QDir getSourceFolder(){
        return sourceFolder_;
    }
    QStringList getFileList();

    QList<QMediaContent> getQueue();
    void addSongToBegining(QString filename);
    void setCurrentSong(QString fileName);
    QMediaPlayer::State getState();

private:
    QMediaPlayer* player_;
    QMediaPlaylist* playlist_;
    //Phonon::MediaObject* playlist_;
    //Phonon::AudioOutput* output_;
    QDir sourceFolder_;


    QAudioInput* input_;
    QFile outputFile;
    QAudioFormat format;
    QBuffer* buffer_;
    QBuffer* inputBuffer_;
    QList<QBuffer*> allBuffers_;
signals:

public slots:
void play();
void pause();
void stop();
void next();
void previous();

void startMic();
void startMic(QIODevice);
void stopMic();

};

#endif // AUDIOCOMPONENT_H
