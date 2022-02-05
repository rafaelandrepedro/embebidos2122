#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>


<<<<<<< Updated upstream
#define PORTNUM 5003
=======
#define PORTNUM 5004
>>>>>>> Stashed changes
#define CONNECTION_NUM 2

void panic(char *msg);
#define panic(m)	{perror(m); abort();}

class WifiCOM{
    private:
        int socket_app;
        bool connection_state;

        int listen_sd;

    public:
        WifiCOM();
        ~WifiCOM();
        void init();
        void close();
        bool connectWifi();
        void sendApp(char[], int size);
        void recvApp(char[], int size);
        int send_image();
};

#endif // WIFI_H
