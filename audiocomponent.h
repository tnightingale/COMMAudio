#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <QObject>

#include <QDir>
#include <QAudioInput>
#include <QAudioOutput>
#include <QBuffer>
#include <QtMultimediaKit/QMediaPlayer>
#include <QtMultimediaKit/QMediaPlaylist>
#include <QtMultimediaKit/QMediaObject>
#include "socket.h"
#include <QThread>

class AudioComponent : public QObject
{
    Q_OBJECT
public:
    /**
    * Constructor.
    *@author Ian Lee
    */
    explicit AudioComponent(QObject *parent = 0);


    /**
    * Add song -filename- to mediaplayer's playlist.
    *@author Ian Lee
    */
    bool addSong(QString filename);
    /**
    * Allows user to select a folder from which a song list is created.
    *@author Ian Lee
    */
    void setSourceFolder();
    /**
    * getter.
    *@author Ian Lee
    */
    QDir getSourceFolder(){
        return sourceFolder_;
    }
    /**
    * recovers a filelist from the source folder.
    *@author Ian Lee
    */
    QStringList getFileList();

    /**
    * gets the playlist.
    *@author Ian Lee
    */
    QList<QMediaContent> getQueue();
    /**
    * adds song -filename- to beginning of playlist.
    *@author Ian Lee
    */
    bool addSongToBegining(QString filename);
    /**
    * inserts song in playlist next and then does next.
    *@author Ian Lee
    */
    void setCurrentSong(QString fileName);
    /**
    * getter.
    *@author Ian Lee
    */
    QMediaPlayer::State getState();
    /**
    * gets player.
    *@author Ian Lee
    */
    QMediaPlayer* getPlayer();
    /**
    * goes to previous song.
    *@author Ian Lee
    */
    void previousIndex(int desiredIndex);
    /**
    * goes to next song.
    *@author Ian Lee
    */
    void nextIndex(int desiredIndex);
    /**
    * goes to desired index.
    *@author Ian Lee
    */
    void gotoIndex(int index);
    /**
    * getter.
    *@author Ian Lee
    */
    QMediaPlaylist* getPlaylist();
    /**
    * getter.
    *@author Ian Lee
    */
    int getIndex();
    /**
    * sets up buffers for multicasting.
    *@author Ian Lee
    */
    void joinMulticast();


private:
    QMediaPlayer* player_;
    QMediaPlaylist* playlist_;
    QIODevice *micIO_;
    QIODevice *speakersIO_;

    QDir sourceFolder_;

    QAudioInput* input_;
    QAudioOutput* output_;
    QFile outputFile;
    QAudioFormat format;
    QIODevice* buff;
    QList<QList<QByteArray*>*> allBuffers_;
    QList<QAudioFormat> allFormats_;

    bool isMicOn;
    bool isSpeakersOn;
signals:

public slots:
    /**
    * adds data from socket to audioOutput and plays it.
    * for multicast client.
    *@author Ian Lee
    */
    void addFromMulticast(Socket* socket);
    /**
    * handles song changing for multicasting.
    *@author Ian Lee
    */
    void stateChangeStream(QAudio::State);


    /**
    * play audio.
    *@author Ian Lee
    */
    void play();
    /**
    * pause audio.
    *@author Ian Lee
    */
    void pause();
    /**
    * stop audio.
    *@author Ian Lee
    */
    void stop();
    /**
    * next song.
    *@author Ian Lee
    */
    void next();
    /**
    * previous song.
    *@author Ian Lee
    */
    void previous();
    /**
    * error handling for microphone.
    * voice chat.
    *@author Tom Nightingale, Luke Queenan, Ian Lee
    */
    void mic(QAudio::State);
    /**
    * error handling for speakers.
    * voice chat
    *@author Tom Nightingale, Luke Queenan, Ian Lee
    */
    void speak(QAudio::State);
    /**
    * starts microphone and hooks it up to socket.
    * voice chat
    *@author Ian Lee
    */
    void startMic(QIODevice* stream, QThread* socketThread);
    /**
    * pauses the microphone.
    *@author Ian Lee
    */
    void pauseMic();
    /**
    * resumes the microphone.
    *@author Ian Lee
    */
    void resumeMic();
    /**
    * stops the microphone.
    *@author Ian Lee
    */
    void stopMic();
    /**
    * disconnects speakers.
    * voice chat.
    *@author Tom Nightingale, Luke Queenan, Ian Lee
    */
    void stopStream();
    /**
    * hooks the speakers up to the socket.
    * voice chat
    *@author Ian Lee
    */
    void playStream(QIODevice* stream, QThread* socketThread);
};

#endif // AUDIOCOMPONENT_H
