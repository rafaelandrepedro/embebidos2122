#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <pthread.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>


void panic(char *msg);

#define panic(m)		{perror(m); abort();}
#define MAX_CLIENT_NUM		10
#define MAX_MSG_LEN     	(1<<12)
#define AFK_TIME 		60000

typedef enum{
	OFFLINE=0,
	ONLINE=1,
	AFK=2
} State;

typedef struct client_socket_into client_socket_info_t;
struct client_socket_into
{
	int socket;		//client socket
	State state;		//client state (ONLINE/OFFLINE)
	int index;		//client number
	clock_t lastActivity;	//last activity from the client
	char clientMode;	//send/recieve/both
}; 

char connections=0;
client_socket_info_t socket_table[MAX_CLIENT_NUM];

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
//thread to recieve and broadcast the messages
void *taskBroadcast(void *arg)                    
{
	char buffer[MAX_MSG_LEN ];
	char bufferf[MAX_MSG_LEN ];
	
	client_socket_info_t *info = (client_socket_info_t *)arg;   				//get client info	
	
	while(recv(info->socket,buffer,sizeof(buffer),0)>0){  					//wait for commands while checking connectivity
		if(strcmp(buffer,"Ping")==0){							//ping response
			//printf("Client %d is ONLINE\n",info->index);
			socket_table[info->index].state = ONLINE;
		}
		else if(buffer[0]=='/'){							//client mode command
			printf("Client %d changed to mode %c\n",info->index,buffer[6]);
			socket_table[info->index].clientMode = buffer[6];	
		}
		else if(info->clientMode=='S' || info->clientMode=='B'){			//message
			printf("Received from Client %d: %s\n",info->index,buffer);
			snprintf(bufferf, MAX_MSG_LEN, "Client %d: %s",info->index,buffer); 	//add the client identification
			
			socket_table[info->index].lastActivity=clock(); 			//update activity
			
			for(int i=0;i<MAX_CLIENT_NUM;i++){					//broadcast
				if(socket_table[i].state==ONLINE && (socket_table[i].clientMode=='R' || socket_table[i].clientMode=='B') && info->index!=i){	//check eligible clients
					printf("Resend to client%d\n",socket_table[i].index);
					send(socket_table[i].socket,bufferf,sizeof(buffer),0);
	    			}
		    	}
	    	}
    	
	}
	    
	
	printf("Close connection: Client %d\n", info->index);		//close client connection
	shutdown(info->socket,SHUT_RD);
	shutdown(info->socket,SHUT_WR);
	shutdown(info->socket,SHUT_RDWR);
	
	
	socket_table[info->index].socket = -1;				//clear the client data
	socket_table[info->index].state = OFFLINE;
	socket_table[info->index].index = -1;
	socket_table[info->index].lastActivity = clock();
}
	                          
//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
//thread to scan for commands in the terminal
void *taskScanTerminal(void *arg){	
	while(1){
		int sd = *(int*)arg;			//get socket

		char buffer[MAX_MSG_LEN ];
		char bufferf[MAX_MSG_LEN ];

		scanf("%s",buffer);
		snprintf(bufferf, MAX_MSG_LEN, "Server: %s",buffer); 	//add the server identification
		for(int i=0;i<connections;i++){ 			//broadcast
			if(socket_table[i].state && (socket_table[i].clientMode=='R' || socket_table[i].clientMode=='B')){	//check eligible clients
				send(socket_table[i].socket,bufferf,sizeof(bufferf),0);
	    		}
	    	}
	}		                                
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
//thread to ping clients
static void checkState(int signo){		
	//printf("checking state\n");
	if(signo==SIGALRM){	//alarm signal
		char buffer[MAX_MSG_LEN]="Ping";
    		for(int i=0;i<connections;i++){						//send Ping
    			if(socket_table[i].state== ONLINE){
    				if(send(socket_table[i].socket,buffer,sizeof(buffer),0)>0 ){
					if( clock() > socket_table[i].lastActivity+AFK_TIME){		//afk
    						printf("Put AFK: Client %d\n",socket_table[i].index);
						socket_table[i].state = AFK;
					}
					else{								//online
						
					}
    				}
				else{									//offline
    						
					printf("Close connection: Client %d\n", socket_table[i].index);		//close client connection
					shutdown(socket_table[i].socket,SHUT_RD);
					shutdown(socket_table[i].socket,SHUT_WR);
					shutdown(socket_table[i].socket,SHUT_RDWR);
	
	
					socket_table[i].socket = -1;				//clear the client data
					socket_table[i].state = OFFLINE;
					socket_table[i].index = -1;
					socket_table[i].lastActivity = clock();
    				}
    			}
    		}
	}
	else if(signo==SIGINT){	//interrupt signal
		for(int i=0;i<connections;i++){								//close server
    			if(socket_table[i].state!=OFFLINE){
				printf("Close connection: Client %d\n",socket_table[i].index);		//close client connection
				shutdown(socket_table[i].socket,SHUT_RD);
				shutdown(socket_table[i].socket,SHUT_WR);
				shutdown(socket_table[i].socket,SHUT_RDWR);
	
				socket_table[i].socket = -1;				//clear the client data
				socket_table[i].state = OFFLINE;
				socket_table[i].index = -1;
				socket_table[i].lastActivity = clock();
			}
		}		
		exit(1);
		
	}	                      
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
int main(int count, char *args[])
{	
	//MAIN SETUP
	struct sockaddr_in addr;
	int listen_sd, port;
	if ( count != 2 )
	{
		printf("usage: %c <protocol or portnum>\n", args[0]);
		exit(0);
	}

	/*---Get server's IP and standard service connection--*/
	if ( !isdigit(args[1][0]) )
	{	
		struct servent *srv = getservbyname(args[1], "tcp");
		if ( srv == NULL )
			panic(args[1]);
		printf("%s: port=%d\n", srv->s_name, ntohs(srv->s_port));
		port = srv->s_port;
	}
	else					 //Se for um digito
		port = htons(atoi(args[1])); //Seleção da porta
		
	printf("Using port %d\n", port);
	
	/*--- create socket ---*/
	listen_sd = socket(PF_INET, SOCK_STREAM, 0);  // Socket Listen
	if ( listen_sd < 0 )
		panic("socket");

	/*--- bind port/address to socket ---*/
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = port;
	addr.sin_addr.s_addr = INADDR_ANY;                  /* any interface */
	

	if ( bind(listen_sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
		panic("bind");

	/*--- make into listener with 10 slots ---*/
	if ( listen(listen_sd, MAX_CLIENT_NUM) != 0 )
		panic("listen")

	/*--- begin waiting for connections ---*/
	else
	{	
		int sd=0;							
		pthread_t scanTerminalThread;
		pthread_create(&scanTerminalThread, 0, taskScanTerminal,&sd);			//create terminal thread
		pthread_detach(scanTerminalThread);
	
		signal(SIGALRM,checkState); 							// set signal (alarm) to trigger every 5 sec
		struct itimerval itv;
		itv.it_interval.tv_sec = 5;
		itv.it_interval.tv_usec = 0;
		itv.it_value.tv_sec = 5;
		itv.it_value.tv_usec = 0;
		setitimer (ITIMER_REAL, &itv, NULL);
		
		//MAIN LOOP
		while (1)		//search for clients
		{
			int n = sizeof(addr);
			int clientsd = accept(listen_sd, (struct sockaddr*)&addr, &n);	//search and accept connections (up to 10)
			if(clientsd!=-1)						//client found
			{
				/*Create socket*/

				printf("New connection: Client %d\n",connections);
				
				socket_table[connections].socket=clientsd;		//set client asconnected
				socket_table[connections].state = ONLINE;
				socket_table[connections].index=connections;
				socket_table[connections].lastActivity=clock();		//update activity
				client_socket_info_t *info;
				info = malloc (sizeof (client_socket_info_t));
				info = &socket_table[connections];
				
				pthread_t broadcastThread;
				pthread_create(&broadcastThread, 0, taskBroadcast, info);//create broadcast thread
				pthread_detach(broadcastThread);

				connections++;
				if(connections==10)					//circular buffer
					connections=0;
			}
			
		}
	}
}
