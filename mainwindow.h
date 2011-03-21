#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QDebug>
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

private:
    Ui::MainWindow *ui;
    TCPSocket *controlSocket_;

private slots:
    void on_action_Visible_toggled(bool status);
    void on_clientListWidget_itemDoubleClicked(QListWidgetItem* item);
};

#endif // MAINWINDOW_H
