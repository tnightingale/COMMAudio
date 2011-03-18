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

public:
    Workstation(MainWindow * mainWindow);
    virtual ~Workstation();

};
