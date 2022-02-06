#include "wifi.h"

void panic(char *msg);
#define panic(m)	{perror(m); abort();}

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

int WifiCOM::send_image()
{

   FILE *picture;
   int size, read_size, stat, packet_index;
   char send_buffer[10240], read_buffer[256];
   packet_index = 1;

   picture = fopen("image.jpeg", "r");

   if(picture == NULL) 
        printf("Error Opening Image File"); 

   fseek(picture, 0, SEEK_END);
   size = ftell(picture);
   fseek(picture, 0, SEEK_SET);

   //Send Picture Size
   write(socket_app, (void *)&size, sizeof(int));

   //Read while we get errors that are due to signals.    
   do 
   { 
      stat=read(socket_app, &read_buffer , 255);
   } 
   while (stat < 0);


   while(!feof(picture))
   {
      //while(packet_index = 1){
      //Read from the file into our send buffer
      read_size = fread(send_buffer, 1, sizeof(send_buffer)-1, picture);

      //Send data through our socket 
      do
      {
        stat = write(socket_app, send_buffer, read_size);  
      }
      while (stat < 0);

      packet_index++;  

      //Zero out our send buffer
      bzero(send_buffer, sizeof(send_buffer));
     }
}
