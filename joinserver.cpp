/*
-- SOURCE FILE: joinserver.cpp
--
-- PROGRAM: Pelican Chat Program
--
-- FUNCTIONS:
-- explicit JoinServer(QWidget *parent = 0);
-- ~JoinServer();
--
-- DATE: March 13, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Luke Queenan
--
-- PROGRAMMER: Luke Queenan
--
-- NOTES:
-- This file contains the logic for receiving information regarding the desired
-- server. The user is prompted for an IP address and a user name.
*/
#include "joinserver.h"
#include "ui_joinserver.h"

/*
-- FUNCTION: JoinServer
--
-- DATE: March 13, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Qt
--
-- PROGRAMMER: Qt
--
-- INTERFACE: explicit JoinServer(QWidget *parent = 0);
--
-- RETURNS:
--
-- NOTES:
-- This is the constructor for the server input window of the chat client.
*/
JoinServer::JoinServer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinServer)
{
    ui->setupUi(this);
}

/*
-- FUNCTION: ~JoinServer
--
-- DATE: March 13, 2011
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Qt
--
-- PROGRAMMER: Qt
--
-- INTERFACE: ~JoinServer();
--
-- RETURNS:
--
-- NOTES:
-- This is the destructor for the join server window of the chat client.
*/
JoinServer::~JoinServer()
{
    delete ui;
}

QString JoinServer::getIp()
{
    return ui->lineEditIp->text();
}

int JoinServer::getPort()
{
    return ui->lineEditPort->text().toInt();
}
