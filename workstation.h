#include <QObject>
#include <QThread>
#include <QMap>

class MainWindow;
class TCPSocket;
class UDPSocket;

class Workstation : public QObject {
  Q_OBJECT

private:
    /**  */
    TCPSocket *tcpSocket_;

    /**  */
    UDPSocket *udpSocket_;

    /* Pointer to the main window. This is used to access the window handle for
    when we create new sockets.*/
    MainWindow *mainWindowPointer_;

    // Collection for file transfers
    QMap <TCPSocket*, QByteArray> currentTransfers;

    // Functions
    void sendFile();
    void sendFileList();
    // Create, send back socket information,
    // create buffer, connect signals/slots
    void acceptVoiceChat();

    bool processReceivingFile();
    bool processReceivingFileList(TCPSocket*, QByteArray*);


public slots:
    // Triggered by user, so signal is coming from a button
    void connectToServer();
    void requestFile();
    void requestFileList();
    // Triggered by sockets, so signal is coming from a socket
    void processConnection(TCPSocket*);
    void decodeControlMessage(TCPSocket*);
    void receiveUDP();
    void receiveFileController(TCPSocket*);
    void receiveFileListController(TCPSocket*);
    void requestFileListController(TCPSocket*);

signals:
    void signalFileListUpdate(QStringList*);

public:
    Workstation(MainWindow *mainWindow);
    virtual ~Workstation();
};
