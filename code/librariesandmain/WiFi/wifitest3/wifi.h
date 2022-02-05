#ifndef WIFI_H
#define WIFI_H


#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>


#define PORTNUM 5004
#define CONNECTION_NUM 2

//const char server_ip[] = "192.168.1.106";

//const char server_ip[] = "192.168.1.98";
const char server_ip[] = "192.168.1.135";
//const char server_ip[] = "192.168.1.102";

void panic(char *msg);
#define panic(m)	{perror(m); abort();}


class Wifi
{
  
public:
    Wifi();
    ~Wifi();	
    void init();
    int connectToServer();
    void receiveFromServer(char [], int);
    int receive_image();




private:
    bool connected;

};

#endif // WIFI_H

