#include <QObject>
#include <QThread>
#include <QMap>
#include <QByteArray>
#include "multicastsession.h"

class MainWindow;
class Socket;
class TCPSocket;
class UDPSocket;
class FileData;
class AudioComponent;

class Workstation : public QObject {
    Q_OBJECT

private:
    /**
    * The 'server' socket that remains open for the programs duration.
    *
    * @author Luke Queenan
    */
    TCPSocket *tcpSocket_;

    /**
     * The control socket for voice control. The request for the voice
     * connection is sent out across this socket, and when the voice chat ends,
     * this socket closes.
     *
     * @author Luke Queenan
     */
    TCPSocket *voiceControlSocket_;

    /**
     * The general purpose UDP socket. This socket is used for sending and
     * receiving voice data, along with multicsting sending or receiving.
     *
     * @author Luke Queenan
     */
    UDPSocket* udpSocket_;

    /**
     * This thread holds all the general purpose sockets, such as file list
     * transfer and actual file transfer.
     *
     * @author Luke Queenan
     */
    QThread* socketThread_;

    /**
     * Pointer to the multicast session object.
     *
     * @author Luke Queenan
     */
    MulticastSession* multicastSession_;

    /**
     * The thread holding the voice in audio.
     *
     * @author Luke Queenan
     */
    QThread* voiceInThread_;

    /**
     * The thread for holding the voice out audio
     *
     * @author Luke Queenan
     */
    QThread* voiceOutThread_;

    /**
     * Pointer to the main window, used for creating new sockets and calling
     * functions in the application layer.
     *
     * @author Luke Queenan
     */
    MainWindow *mainWindowPointer_;

    /**
     * The map containing any current file transfer objects.
     *
     * @author Luke Queenan
     */
    QMap <Socket*, FileData*> currentTransfers;

    // Functions
    /**
     * This function opens a file, reads it into a QByteArray, inserts its size
     * at the front of the array and writes it to the supplied socket.
     *
     * @author Luke Queenan
     */
    void sendFile(Socket*, QByteArray*);

    /**
     * This function is called when an incoming voice call is detected. It
     * prompts the user for a response and then sets up the voice call (if the
     * user said yes).
     *
     * @author Luke Queenan
     */
    void acceptVoiceChat(Socket*);

    /**
     * Processes a file packet. If it is the first packet, the size of the file
     * is stripped off the front of the packet and stored. If it is the last
     * packet, the file is written to disk in the file transfer object.
     * Otherwise the data is appended to the file data's internal array.
     *
     * @author Luke Queenan
     */
    bool processReceivingFile(Socket*, QByteArray*);

    /**
     * Process a file list packet. Same format as above.
     *
     * @author Luke Queenan
     */
    bool processReceivingFileList(Socket*, QByteArray*);

    /**
     * Same format as above.
     *
     * @author Luke Queenan
     */
    bool processReceivingFileRequest(Socket*, QByteArray*);


public slots:
    // Triggered by GUI buttons
    /**
     * Triggered by the GUI when the user requests a file
     *
     * @author Luke Queenan
     */
    void requestFile(QString, short, QString);
    /**
     * Triggered by the GUI when the user wishes to exchange file lists
     *
     * @author Luke Queenan
     */
    void requestFileList(QString, short);
    /**
     * Triggered by the GUI when the user wishes to end a voice chat session
     *
     * @author Luke Queenan
     */
    void endVoiceStreamUser();
    // Triggered by sockets
    /**
     * Connects signals and opens the socket for read write
     *
     * @author Luke Queenan
     */
    void processConnection(Socket*);
    /**
     * Takes a newly connected socket and determines its purpose based on the
     * first byte.
     *
     * @author Luke Queenan
     */
    void decodeControlMessage(Socket*);
    /**
     * The controller for receiving a file. If the file has been received, then
     * the socket is destroyed.
     *
     * @author Luke Queenan
     */
    void receiveFileController(Socket*);
    /**
     * The controller for receiving a file list. If the file list has been
     * received, our own list is sent out.
     *
     * @author Luke Queenan
     */
    void receiveFileListController(Socket*);
    /**
     * Receives the other clients file list
     *
     * @author Luke Queenan
     */
    void requestFileListController(Socket*);
    /**
     * The send file controller that receives the file name to send out.
     *
     * @author Luke Queenan
     */
    void sendFileController(Socket*);
    /**
     * Triggered when the user wishes to start a voice chat. Creates the
     * resources and sends the request packet.
     *
     * @author Luke Queenan
     */
    void initializeVoiceStream(short port, QString hostAddr, AudioComponent* player);
    /**
     * Triggered when the voice control connection is disconnected, which causes
     * the voice chat to terminate.
     *
     * @author Luke Queenan
     */
    void endVoiceStream();
    /**
     * This starts a multicast session by creating the sockets and connecting
     * the signals and slots
     *
     * @author Luke Queenan
     */
    void startMulticast(QStringList* list);
    /**
     * Joins a multicast session by creating the socket and connecting signals
     * and slots.
     *
     * @author Luke Queenan
     */
    void joinMulticast(QString address,int port);
    /**
     * Leaves the multicast session by destroying the socket and shutting off
     * the audio.
     *
     * @author Luke Queenan
     */
    void leaveMulticast();

signals:
    void signalFileListUpdate(QStringList*);
    void signalFileProgress(int, int, QString);

public:
    Workstation(MainWindow *mainWindow);
    virtual ~Workstation();

};
