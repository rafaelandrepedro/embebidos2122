#include "wifi.h"



int listen_sd;
struct sockaddr_in addr;
struct hostent* host;
int port;

Wifi::Wifi()
{
}

Wifi::~Wifi()
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

int Wifi::connectToServer()
{

    if(!connected)
    {
        connect(listen_sd, (struct sockaddr*)&addr, sizeof(addr));
        return 1;
    }
    else
    	return 0;

}


void Wifi::receiveFromServer(char message[], int size)
{
    recv(listen_sd, message, size,0);
}



int Wifi::receive_image()
{
    int recv_size = 0,size = 0, read_size, write_size, packet_index =1,stat;
    char imagearray[10241],verify = '1';
    FILE *image;

    //Get size of the image
    do
    {
    	stat = read(listen_sd, &size, sizeof(int));
    }
    while(stat<0);


    char buffer[] = "Got it";

    //Send our verification signal
    do
    {
        stat = write(listen_sd, &buffer, sizeof(int));
    }
    while(stat<0);


    image = fopen("image.jpeg", "w");

    if( image == NULL)
    {
    	printf("Error oppening File\n");
	return -1;
    }

    //Loop while we have not received the entire file yet


    int need_exit = 0;
    struct timeval timeout = {10,0};

    fd_set fds;
    int buffer_fd, buffer_out;

    while(recv_size < size) 
    {

        FD_ZERO(&fds);
        FD_SET(listen_sd,&fds);

        buffer_fd = select(FD_SETSIZE,&fds,NULL,NULL,&timeout);

        if (buffer_fd < 0)
           printf("error: bad file descriptor set.\n");

        if (buffer_fd == 0)
           printf("error: buffer read timeout expired.\n");

        if (buffer_fd > 0)
        {
            do
            {
                read_size = read(listen_sd,imagearray, 10241);
            }
            while(read_size <0);

            //Write the currently read data into our image file
            write_size = fwrite(imagearray,1,read_size, image);
            

            if(read_size !=write_size)
            {
                 printf("error in read write\n");
            }


            //Increment the total number of bytes read
            recv_size += read_size;
            packet_index++;
        }

    }


    fclose(image);
    printf("Image successfully Received!\n");
    return 1;
  }


