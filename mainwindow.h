#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "tcpsocket.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * Expose the windows message loop for us to respond to socket events.
     * The first of many attrocities...
     *
     * @param msg The windows message received.
     * @param result ... not sure >_>
     *
     * @author Tom Nightingale
     */
    bool winEvent(MSG * msg, long * result);

private:
    Ui::MainWindow *ui;
    TCPSocket *controlSocket_;

signals:
    void signalWMWSASyncTCPRx(PMSG);
    void signalWMWSASyncUDPRx(PMSG);

private slots:
    void on_action_Visible_toggled(bool status);
};

#endif // MAINWINDOW_H
