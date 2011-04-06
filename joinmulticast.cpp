#include "joinmulticast.h"
#include "ui_joinmulticast.h"

JoinMulticast::JoinMulticast(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinMulticast)
{
    ui->setupUi(this);
    QWidget::setWindowIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    this->setFixedSize(407,57);
}

JoinMulticast::~JoinMulticast()
{
    delete ui;
}
void JoinMulticast::setPort(int port){
    port_ = port;
}

void JoinMulticast::setIp(QString ip){
    ui->ipField->setText(ip);
    emit play(ip, port_);
}
