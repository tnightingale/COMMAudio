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
    void connectToServer();
    void requestFile(QString, short, QString);
    void requestFileList(QString, short);
    void endVoiceStreamUser();
    // Triggered by sockets
    void processConnection(Socket*);
    void decodeControlMessage(Socket*);
    void receiveUDP();
    void receiveFileController(Socket*);
    void receiveFileListController(Socket*);
    void requestFileListController(Socket*);
    void sendFileController(Socket*);
    void initializeVoiceStream(short port, QString hostAddr, AudioComponent* player);
    void endVoiceStream();
    void startMulticast(QStringList* list);
    void joinMulticast(QString address,int port);
    void leaveMulticast();

signals:
    void signalFileListUpdate(QStringList*);
    void signalFileProgress(int, int, QString);

public:
    Workstation(MainWindow *mainWindow);
    virtual ~Workstation();

};
