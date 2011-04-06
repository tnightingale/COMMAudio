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
    /**  */
    TCPSocket *tcpSocket_;

    TCPSocket *voiceControlSocket_;

    /**  */
    UDPSocket* udpSocket_;

    QThread* socketThread_;
    MulticastSession* multicastSession_;
    QThread* voiceInThread_;
    QThread* voiceOutThread_;

    /* Pointer to the main window. This is used to access the window handle for
    when we create new sockets.*/
    MainWindow *mainWindowPointer_;

    // Collection for file transfers
    QMap <Socket*, FileData*> currentTransfers;

    // Functions
    void sendFile(Socket*, QByteArray*);
    void acceptVoiceChat(Socket*);

    bool processReceivingFile(Socket*, QByteArray*);
    bool processReceivingFileList(Socket*, QByteArray*);
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
