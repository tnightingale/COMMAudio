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


    bool addSong(QString filename);
    void setSourceFolder();
    QDir getSourceFolder(){
        return sourceFolder_;
    }
    QStringList getFileList();

    QList<QMediaContent> getQueue();
    bool addSongToBegining(QString filename);
    void setCurrentSong(QString fileName);
    QMediaPlayer::State getState();
    QMediaPlayer* getPlayer();
    void previousIndex(int desiredIndex);
    void nextIndex(int desiredIndex);
    void gotoIndex(int index);
    QMediaPlaylist* getPlaylist();
    int getIndex();
    void testwav(QString fileName);
    void addFromMulticast(QIODevice* socket);
    void writeToMulticast(QString fileName, QIODevice* socket);

    void joinMulticast();
private:
    QMediaPlayer* player_;
    QMediaPlaylist* playlist_;
    //Phonon::MediaObject* playlist_;
    //Phonon::AudioOutput* output_;
    QDir sourceFolder_;

QByteArray data;
    QAudioInput* input_;
    QAudioOutput* output_;
    QFile outputFile;
    QAudioFormat format;
    QBuffer* buffer_;
    QIODevice* buff;
    QBuffer* inputBuffer_;
    QList<QByteArray> allBuffers_;
signals:

public slots:
void play();
void pause();
void stop();
void next();
void previous();

void addToOutput(QAudio::State);
void checkBuff();

void startMic();
void startMic(QIODevice* stream);
void stopMic();
void playStream(QIODevice* stream);
};

#endif // AUDIOCOMPONENT_H
