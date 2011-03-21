#include <QObject>

class MainWindow;
class TCPSocket;
class UDPSocket;

class Workstation : public QObject {
  Q_OBJECT

private:
    /**  */
    TCPSocket * tcpSocket_;

    /**  */
    UDPSocket * udpSocket_;

    // Functions
    void requestFileList();
    void sendFile();
    void sendFileList();
    // Create, send back socket information,
    // create buffer, connect signals/slots
    void acceptVoiceChat();

public slots:
    // Tied to UI
    void connectToServer();
    void requestFile();
    // Tied to sockets
    void processControlMessage();

    void receiveUDP();
    void receiveFile();

public:
    Workstation(MainWindow * mainWindow);
    virtual ~Workstation();
};
