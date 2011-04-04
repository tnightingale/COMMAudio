#include "joinmulticast.h"
#include "ui_joinmulticast.h"

JoinMulticast::JoinMulticast(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinMulticast)
{
    ui->setupUi(this);
    this->setFixedSize(407,57);
}

JoinMulticast::~JoinMulticast()
{
    delete ui;
}

void JoinMulticast::setIp(QString ip){
    ui->ipField->setText(ip);
}
