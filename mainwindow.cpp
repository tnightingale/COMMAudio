#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Visible_toggled(bool status)
{
    if (status)
    {
        // Create the listen socket
        return;
    }
    else
    {
        // Either close the socket here or find a way to hide it
        return;
    }
}

bool MainWindow::winEvent(MSG * msg, long * result) {
    switch (msg->message) {
        case WM_WSAASYNC_TCP:
            emit signalWMWSASyncTCPRx(msg);
            return true;

        case WM_WSAASYNC_UDP:
            emit signalWMWSASyncUDPRx(msg);
            return true;
    }

    return false;
}

