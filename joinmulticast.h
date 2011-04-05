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
signals:
    void play(QString ip);
private:
    Ui::JoinMulticast *ui;
};

#endif // JOINMULTICAST_H
