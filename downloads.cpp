#include "downloads.h"
#include "ui_downloads.h"

Downloads::Downloads(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Downloads)
{
    ui->setupUi(this);
    ui->download1->setReadOnly(true);
    ui->download2->setReadOnly(true);
    ui->download3->setReadOnly(true);
    ui->download4->setReadOnly(true);
    ui->download5->setReadOnly(true);
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
                ui->download1->clear();
                ui->downloadbar1->reset();

        }
    } else if(ui->download2->toPlainText() == file) {
        ui->downloadbar2->setMaximum(filesize);
        ui->downloadbar2->setValue(ui->downloadbar2->value() + packetsize);
        if(ui->downloadbar2->value() == ui->downloadbar2->maximum()) {
            ui->download2->clear();
            ui->downloadbar2->reset();
        }
    }else if(ui->download3->toPlainText() == file) {
        ui->downloadbar3->setMaximum(filesize);
        ui->downloadbar3->setValue(ui->downloadbar3->value() + packetsize);
        if(ui->downloadbar3->value() == ui->downloadbar3->maximum()) {
            ui->download3->clear();
            ui->downloadbar3->reset();
        }
    }else if(ui->download4->toPlainText() == file) {
        ui->downloadbar4->setMaximum(filesize);
        ui->downloadbar4->setValue(ui->downloadbar4->value() + packetsize);
        if(ui->downloadbar4->value() == ui->downloadbar4->maximum()) {
            ui->download4->clear();
            ui->downloadbar4->reset();

        }
    } else if(ui->download5->toPlainText() == file) {
        ui->downloadbar5->setMaximum(filesize);
        ui->downloadbar5->setValue(ui->downloadbar5->value() + packetsize);
        if(ui->downloadbar5->value() == ui->downloadbar5->maximum()) {
             ui->download5->clear();
            ui->downloadbar5->reset();
        }
    } else if(ui->download1->toPlainText().size() <= 0) {
        ui->download1->setText(file);
        ui->downloadbar1->setMaximum(filesize);
        ui->downloadbar1->setValue(packetsize);
    }else if (ui->download2->toPlainText().size() <= 0) {
        ui->download2->setText(file);
        ui->downloadbar2->setMaximum(filesize);
        ui->downloadbar2->setValue(packetsize);
    }else if (ui->download3->toPlainText().size() <= 0) {
        ui->download3->setText(file);
        ui->downloadbar3->setMaximum(filesize);
        ui->downloadbar3->setValue(packetsize);
    } else if (ui->download4->toPlainText().size() <= 0) {
        ui->download4->setText(file);
        ui->downloadbar4->setMaximum(filesize);
        ui->downloadbar4->setValue(packetsize);
    }else if (ui->download5->toPlainText().size() <= 0) {
        ui->download5->setText(file);
        ui->downloadbar5->setMaximum(filesize);
        ui->downloadbar5->setValue(packetsize);
    }
    if(countCurrentDownloads() < 5) {
       emit queueFull(false);
    }else{
        emit queueFull(true);
    }
}

int Downloads::countCurrentDownloads(){
    int count = 0;
    if(ui->download1->toPlainText().length() > 0) {
        count++;
    }
    if(ui->download2->toPlainText().length() > 0) {
        count++;
    }
    if(ui->download3->toPlainText().length() > 0) {
        count++;
    }
    if(ui->download4->toPlainText().length() > 0) {
        count++;
    }
    if(ui->download5->toPlainText().length() > 0) {
        count++;
    }
    return count;
}
