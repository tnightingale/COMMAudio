#include <QtGui/QApplication>
#include "workstation.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Workstation * ws = new Workstation(&w);

    w.show();

    return a.exec();
}
