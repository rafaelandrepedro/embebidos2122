#include "wifi.h"

struct sockaddr_in addr;

WifiCOM::WifiCOM(){}

WifiCOM::~WifiCOM(){}

void WifiCOM::init()
{
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

bool WifiCOM::connectWifi()
{
    int n = sizeof(addr);
    socket_app = accept(listen_sd, (struct sockaddr*)&addr, (unsigned int*)&n);

    if (socket_app != -1)
        return 1;
    else
        return 0;

}



int WifiCOM::send_image()
{
     FILE *in = fopen("test.jpeg","r");
    char Buffer[2] = "";
    int len;
    while ((len = fread(Buffer,sizeof(Buffer),1, in)) > 0)
    {            
        send(socket_app,Buffer,sizeof(Buffer),0);            
    }
    send(socket_app,"Hi",sizeof(Buffer),0);

    char Buf[BUFSIZ];
    recv(socket_app, Buf, BUFSIZ, 0);
    if ( strcmp (Buf,"ACK") == 0  )
    {
        printf("Recive ACK\n");
    }        
}


void WifiCOM::sendApp(char msg[], int size)
{
    send(socket_app, msg, size, 0);
}

void WifiCOM::recvApp(char cmd[], int size)
{
    recv(socket_app, cmd, size, 0);
}

void WifiCOM::close()
{
    shutdown(socket_app,SHUT_RD);
    shutdown(socket_app,SHUT_WR);
    shutdown(socket_app,SHUT_RDWR);
}
