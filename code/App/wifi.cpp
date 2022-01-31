#include "wifi.h"
#include <QDebug>


qint16 listen_sd;
struct sockaddr_in addr;
struct hostent* host;
int port;

Wifi::Wifi(QObject *parent) : QObject(parent)
{

}

void Wifi::init()
{
    host = gethostbyname(server_ip);
    port = htons(PORTNUM);

    connected = 0;

    // Create socket
    listen_sd = socket(PF_INET, SOCK_STREAM, 0);
    if (listen_sd < 0 )
        panic("socket")

    /* create & zero struct */
    memset(&addr, 0, sizeof(addr));
    /* select internet protocol */
    addr.sin_family = AF_INET;
    /* set the port # */
    addr.sin_port = port;
    /* set the addr */
    addr.sin_addr.s_addr = *(long*)(host->h_addr_list[0]);

}

void Wifi::connectToServer()
{

    if(!connected)
    {
        ::connect(listen_sd, (struct sockaddr*)&addr, sizeof(addr));
        connected = 1;
    }

}

void Wifi::sendToServer(QString cmd)
{
    char msg[128] = "";
    QByteArray ba = cmd.toLocal8Bit();
    strcpy(msg, ba.data());
    send(listen_sd, msg, sizeof(msg), 0);
}

void Wifi::receiveFromServer()
{
    qDebug() << "here\n";
    return;
    char message[2000] = "";
    qDebug() << message;
    recv(listen_sd, message, 2000,0);
}



