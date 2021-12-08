#include <stdio.h>
#include <sys/socket.h>
#include <mqueue.h>
#include <sys/types.h>
#include <resolv.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <signal.h>

#define MAX_MSG_LEN     (1<<12)

mqd_t msgq_id;
unsigned int sender;
int sd;
void panic(char *msg);
#define panic(m)	{perror(m); abort();}


//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
//thread to get the messages from the server
void *taskPrintBroadcast(void *arg)                    
{	
while(1){
	int sd = *(int*)arg;			//get socket

	char buffer[MAX_MSG_LEN ];
	char client=' ';

	recv(sd,buffer,sizeof(buffer),0);
	switch(buffer[0]){
	case 'S':
		printf("%s\n",buffer);		//print server message
		break;
	case 'C':
		printf("%s\n",buffer);		//print client message
		break;
	case 'P':
		//printf("Pinged by server\n");	//resend ping
		send(sd,buffer,MAX_MSG_LEN,0);
		break;
	}
	
}
	                          /* terminate the thread */
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
//thread to scan for commands in the terminal
void *taskScanTerminal(void *arg)                    
{	
while(1){
	int sd = *(int*)arg;           		//get socket

	char buffer[MAX_MSG_LEN];
	int msgsz;

	scanf("%s",buffer);
    	
    	printf("You sent: %s\n",buffer);	//echo command

	send(sd,buffer,MAX_MSG_LEN,0);		//send command to server
	}
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
//signal handling
static void int_handler(int signo){	
	if(signo==SIGINT){
		shutdown(sd,SHUT_RD);	//disable socket
		shutdown(sd,SHUT_WR);
		shutdown(sd,SHUT_RDWR);
		mq_close(msgq_id);	//close queue
		exit(1);
		
	}
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
int main(int count, char *args[])
{	
    	pid_t pid = getpid();
	printf("Running client in pid %d\n", pid);

   	signal(SIGINT,  int_handler);

	struct hostent* host;
	struct sockaddr_in addr;
	int port;
	


	if ( count != 3 )
	{
		printf("usage: %s <servername> <protocol or portnum>\n", args[0]);
		exit(0);
	}

	/*---Get server's IP and standard service connection--*/
	host = gethostbyname(args[1]);
	//printf("Server %s has IP address = %s\n", args[1],inet_ntoa(*(long*)host->h_addr_list[0]));
	if ( !isdigit(args[2][0]) )
	{
		struct servent *srv = getservbyname(args[2], "tcp");
		if ( srv == NULL )
			panic(args[2]);
		printf("%s: port=%d\n", srv->s_name, ntohs(srv->s_port));
		port = srv->s_port;
	}
	else
		port = htons(atoi(args[2]));

	/*---Create socket and connect to server---*/
	sd = socket(PF_INET, SOCK_STREAM, 0);        /* create socket */
	if ( sd < 0 )
		panic("socket");
	memset(&addr, 0, sizeof(addr));       /* create & zero struct */
	addr.sin_family = AF_INET;        /* select internet protocol */
	addr.sin_port = port;                       /* set the port # */
	addr.sin_addr.s_addr = *(long*)(host->h_addr_list[0]);  /* set the addr */

	/*---If connection successful, send the message and read results---*/
	if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) == 0)
	{	
		
		char* buffer="/mode B";
		send(sd,buffer,MAX_MSG_LEN,0);				//set default client mode
		
		pthread_t child,child2;
		
		pthread_create(&child, 0, taskPrintBroadcast, &sd);	//start broadcasting
		pthread_detach(child);
		pthread_create(&child2, 0, taskScanTerminal, &sd);      //start scanning the terminal
		pthread_detach(child2);
		while(1){}
		
	}
	else
		panic("connect");
}
