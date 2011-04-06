#ifndef MULTICASTSESSION_H
#define MULTICASTSESSION_H

#include <QObject>
#include <QStringList>
#include <QByteArray>
#include <QFile>
#include <QTimerEvent>

class UDPSocket;

/**
 * This class manages hosting of a multicast session (acting as a multicast
 * server). 
 * 
 * It's primary role is to write a list of songs to the multicast address one
 * buffer at a time. Iterating over songs in the list and breaking them into
 * buffers is all encapsulated within this class.
 *
 * @author Tom Nightingale, Ian Lee
 */
class MulticastSession : public QObject {
  Q_OBJECT
private:
    /** The upstream socket that writes to the multicast address. */
    UDPSocket* multicastSocket_;
    
    /** The list of songs to broadcast. */
    QStringList* playlist_;

    /** An iterator used to keep track of current position in the list of 
     * songs and provide the next available song. */
    QStringListIterator* playlistIterator_;

    /** The current file being streamed. It is in an open state for reading. */
    QFile* current_;

    /** The next buffer to be written to the socket. */
    QByteArray* nextBuff_;
    
    /** The header to be attached to the buffer. */
    QByteArray header_;

    /** The id of the active timer being used to time buffer writes. */
    int currentTimer_;


    /**
     * Loads the next buffer. Ensures that there is always a buffer available 
     * write to the socket.
     *
     * @author Tom Nightingale
     */
    void loadBuffer();

    /**
     * Generates the next buffer so that it can be loaded. This attaches the
     * header of the current song to every buffer.
     *
     * @author Tom Nightingale, Ian Lee 
     */
    QByteArray* generateBuffer() ;

protected:
    /**
     * Overloaded function from QObject.
     * Fired every time this object's internal timer emits a timeout event.
     *
     * This initiates the writing of a buffer to the socket and the loading of
     * a new buffer.
     *
     * @param event A pointer to the timer event that triggered this function
     *              call.
     * 
     * @author Tom Nightingale, Ian Lee 
     */
    void timerEvent(QTimerEvent* event);

public:
    /**
     * Constructor.
     * Prepares the socket and playlist and other data members for streaming.
     *
     * @param socket The initialized UDP socket for streaming to.
     * @param playlist A pointer to a list of songs to stream.
     *
     * @author Tom Nightingale, Ian Lee 
     */
    MulticastSession(UDPSocket* socket, QStringList* playlist);

    /**
     * Destructor.
     * Cleans up the resorces used by this object.
     *
     * @author Tom Nightingale, Ian Lee 
     */
    virtual ~MulticastSession();

public slots:
    /**
     * Starts the multicast streaming. Starts the timer ticks.
     *
     * @author Tom Nightingale, Ian Lee 
     */
    void start();

    /**
     * Pauses the multicast streaming. Stops the timer ticks.
     *
     * @author Tom Nightingale, Ian Lee 
     */
    void pause();

    /**
     * Ends the multicast session, makes sure to properly clean up resources.
     *
     * @author Tom Nightingale, Ian Lee 
     */
    void endSession();

signals:

};

#endif
