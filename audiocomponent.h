#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <QObject>
#include <Phonon>
#include <QDir>
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
    QFileInfoList getFileList();


private:
    Phonon::MediaObject* playlist_;
    Phonon::AudioOutput* output_;
    QDir sourceFolder_;

signals:

public slots:
void play();
void pause();
void stop();

};

#endif // AUDIOCOMPONENT_H
