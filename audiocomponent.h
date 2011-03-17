#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <QObject>
#include <Phonon>
class AudioComponent : public QObject
{
    Q_OBJECT
public:
    explicit AudioComponent(QObject *parent = 0);

    void addSong(QString filename);
private:
    Phonon::MediaObject* playlist_;
    Phonon::AudioOutput* output_;
signals:

public slots:
void play();
void pause();
void stop();

};

#endif // AUDIOCOMPONENT_H
