#ifndef JOINSERVER_H
#define JOINSERVER_H

#include <QDialog>

namespace Ui {
    class JoinServer;
}

class JoinServer : public QDialog
{
    Q_OBJECT

public:
    explicit JoinServer(QWidget *parent = 0);
    ~JoinServer();

    // Getters for input boxes
    QString getIp();
    int getPort();

private:
    Ui::JoinServer *ui;
    int test;
};

#endif // JOINSERVER_H
