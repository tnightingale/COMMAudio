#ifndef DOWNLOADS_H
#define DOWNLOADS_H

#include <QDialog>
/*#include "mainwindow.h"
#include "ui_mainwindow.h"
class MainWindow;
*/
namespace Ui {
    class Downloads;
}

class Downloads : public QDialog
{
    Q_OBJECT

public:

    explicit Downloads(QWidget *parent = 0);
    ~Downloads();
    int countCurrentDownloads();
    void downloadFile(int filesize, int packetSize, QString file);

private:
    Ui::Downloads *ui;
   // Ui::MainWindow *mainPointer_;
};

#endif // DOWNLOADS_H
