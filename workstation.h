#include <QObject>
#include <QThread>
#include <QMap>

class MainWindow;
class TCPSocket;
class UDPSocket;
class FileData;

class Workstation : public QObject {
  Q_OBJECT

private:
    /**  */
    TCPSocket *tcpSocket_;

    /**  */
    UDPSocket *udpSocket_;

    QThread* socketThread_;

    /* Pointer to the main window. This is used to access the window handle for
    when we create new sockets.*/
    MainWindow *mainWindowPointer_;

    // Collection for file transfers
    QMap <TCPSocket*, FileData*> currentTransfers;

    // Functions
    void sendFile(TCPSocket* socket);
    // Create, send back socket information,
    // create buffer, connect signals/slots
    void acceptVoiceChat();

    bool processReceivingFile(TCPSocket* socket, QByteArray* packet);
    bool processReceivingFileList(TCPSocket*, QByteArray*);


public slots:
    // Triggered by GUI buttons
    void connectToServer();
    void requestFile(QString, short, QString);
    void requestFileList(QString, short);
    // Triggered by sockets
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
