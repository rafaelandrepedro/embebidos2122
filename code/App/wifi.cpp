#include "wifi.h"
#include <QDebug>

Wifi::Wifi(QObject *parent) : QObject(parent)
{

}

void Wifi::init()
{
    //qDebug() << "Im being called";
    int port;
       port = htons(PORTNUM);

       /*--- create socket ---*/
       listen_sd = socket(PF_INET, SOCK_STREAM, 0);

       if ( listen_sd < 0 )
           panic("socket");

       /*--- bind port/address to socket ---*/
       memset(&addr, 0, sizeof(addr));
       addr.sin_family = AF_INET;
       addr.sin_port = port;
       addr.sin_addr.s_addr = INADDR_ANY;

       if ( bind(listen_sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
           panic("bind");

       /*--- make into listener with MAX_CONNECTIONS slots ---*/
       if ( listen(listen_sd, CONNECTION_NUM) != 0 )
           panic("listen");

}
