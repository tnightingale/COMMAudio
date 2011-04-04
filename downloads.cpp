#include "downloads.h"
#include "ui_downloads.h"

Downloads::Downloads(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Downloads)
{
    ui->setupUi(this);
}


Downloads::~Downloads()
{
    delete ui;
}

void Downloads::downloadFile(int filesize, int packetsize, QString file){
    if(ui->download1->toPlainText() == file) {
        ui->downloadbar1->setMaximum(filesize);
        ui->downloadbar1->setValue(ui->downloadbar1->value() + packetsize);
        if(ui->downloadbar1->value() == ui->downloadbar1->maximum()) {
                ui->downloadbar1->reset();
                ui->download1->clear();
        }
    } else if(ui->download2->toPlainText() == file) {
        ui->downloadbar2->setMaximum(filesize);
        ui->downloadbar2->setValue(ui->downloadbar2->value() + packetsize);
        if(ui->downloadbar2->value() == ui->downloadbar2->maximum()) {
                ui->downloadbar2->reset();
                ui->download2->clear();
        }
    } else if(ui->download3->toPlainText() == file) {
        ui->downloadbar3->setMaximum(filesize);
        ui->downloadbar3->setValue(ui->downloadbar3->value() + packetsize);
        if(ui->downloadbar3->value() == ui->downloadbar3->maximum()) {
                ui->downloadbar3->reset();
                ui->download3->clear();
        }
    } else if(ui->download4->toPlainText() == file) {
        ui->downloadbar4->setMaximum(filesize);
        ui->downloadbar4->setValue(ui->downloadbar4->value() + packetsize);
        if(ui->downloadbar4->value() == ui->downloadbar4->maximum()) {
                ui->downloadbar4->reset();
                ui->download4->clear();
        }
    } else if(ui->download5->toPlainText() == file) {
        ui->downloadbar5->setMaximum(filesize);
        ui->downloadbar5->setValue(ui->downloadbar5->value() + packetsize);
        if(ui->downloadbar5->value() == ui->downloadbar5->maximum()) {
                ui->downloadbar5->reset();
                ui->download5->clear();
        }
    }

    /*if(countCurrentDownloads() < 5) {
        mainPointer_->remoteListWidget->setDisabled(false);
    }else{
        mainPointer_->remoteListWidget->setDisabled(true);
    }
    */
}

int Downloads::countCurrentDownloads(){
    int count = 0;
    if(ui->download1->toPlainText().length() > 0) {
        count++;
    } else if(ui->download2->toPlainText().length() > 0) {
        count++;
    } else if(ui->download3->toPlainText().length() > 0) {
        count++;
    } else if(ui->download4->toPlainText().length() > 0) {
        count++;
    } else if(ui->download5->toPlainText().length() > 0) {
        count++;
    }
    return count;
}

