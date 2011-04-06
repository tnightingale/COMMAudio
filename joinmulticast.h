#ifndef JOINMULTICAST_H
#define JOINMULTICAST_H

#include <QDialog>

namespace Ui {
    class JoinMulticast;
}

class JoinMulticast : public QDialog
{
    Q_OBJECT

public:
    explicit JoinMulticast(QWidget *parent = 0);
    ~JoinMulticast();
    /**
     * sets ip
     *
     * @author Joel Stewart
     */
    void setIp(QString ip);
    void setPort(int port);
signals:
    void play(QString ip,int port);
private:
    Ui::JoinMulticast *ui;
    int port_;
};

#endif // JOINMULTICAST_H
