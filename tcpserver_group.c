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
/*Function to receive messages from each client and send to others (Broadcast)*/
void *taskBroadcast(void *arg)                    
{
	char buffer[MAX_MSG_LEN ];
	char bufferf[MAX_MSG_LEN ];
	
	client_socket_info_t *info = (client_socket_info_t *)arg;   /* get & convert the socket */	
	
	while(recv(info->socket,buffer,sizeof(buffer),0)>0){  /* Verify if still connected and receive*/
		//printf("%d",strcmp(buffer,"Ping"));
		if(strcmp(buffer,"Ping")==0){
			//printf("Client %d is ONLINE\n",info->index);
			socket_table[info->index].state = ONLINE;
		}
		else if(buffer[0]=='/'){
			printf("Client %d changed to mode %c\n",info->index,buffer[6]);
			socket_table[info->index].clientMode = buffer[6];	
		}
		else if(info->clientMode=='S' || info->clientMode=='B'){
			printf("Received from Client %d: %s\n",info->index,buffer);
			snprintf(bufferf, MAX_MSG_LEN, "Client %d: %s",info->index,buffer); /* Add SX to say the sender ex: S1 (Client 1)*/
			
			socket_table[info->index].lastActivity=clock(); /*End time 1 min*/
			
			for(int i=0;i<MAX_CLIENT_NUM;i++){                     /* Send to all clients*/
				if(socket_table[i].state==ONLINE && (socket_table[i].clientMode=='R' || socket_table[i].clientMode=='B') && info->index!=i){    /* Verify IF ative AND not send to the same customer  */
					printf("Resend to client%d\n",socket_table[i].index);
					send(socket_table[i].socket,bufferf,sizeof(buffer),0);  /* Send message*/
	    			}
		    	}
	    	}
    	
	}
	    
	
	printf("Close connection: Client %d\n", info->index);
	shutdown(info->socket,SHUT_RD);
	shutdown(info->socket,SHUT_WR);
	shutdown(info->socket,SHUT_RDWR);
	
	//clear the socket
	socket_table[info->index].socket = -1;
	socket_table[info->index].state = OFFLINE;
	socket_table[info->index].index = -1;
	socket_table[info->index].lastActivity = clock();
	/* terminate the thread */
}
	                          
//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
/*Function to scan terminal and send the message to all clients  */
void *taskScanTerminal(void *arg){	
	while(1){
		int sd = *(int*)arg;               /* get & convert the socket */
		char buffer[MAX_MSG_LEN ];
		char bufferf[MAX_MSG_LEN ];
		scanf("%s",buffer);
		if(buffer[0]=='S' &&buffer[1]=='T'){
			printf("Status: 0-offline || 1-online || 2-AFK \n");
			printf("Client %c has the status: %d\n",buffer[2],socket_table[buffer[2]-'0'].state);
		}else{
		snprintf(bufferf, MAX_MSG_LEN, "Server: %s",buffer); /* Add SX to say the sender ex: S1 (Client 1)*/	
		for(int i=0;i<connections;i++){    /* Send to all clients*/
			if(socket_table[i].state){ /* Verify IF ative*/
				send(socket_table[i].socket,bufferf,sizeof(bufferf),0); /* Send message*/
			}
	    	}
	    	}
	}		                                
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
/*Function to send a predefined message (used to send this message every 1 second)  */
static void checkState(int signo){		
	if(signo==SIGALRM){	/* handle of SIGNAL Alarm*/
		char buffer[MAX_MSG_LEN]="Ping"; /* send simple message*/
    		for(int i=0;i<connections;i++){    /* Send to all clients*/
    			if(socket_table[i].state== ONLINE){ /* Verify IF ative*/
    				if(send(socket_table[i].socket,buffer,sizeof(buffer),0)<0 ){ /* Send message*/
    				
    						printf("Close connection timeout: Client %d\n",socket_table[i].index);
    						
						shutdown(socket_table[i].socket,SHUT_RD);
						shutdown(socket_table[i].socket,SHUT_WR);
						shutdown(socket_table[i].socket,SHUT_RDWR);
						
						//clear the socket
						socket_table[i].socket = -1;
						socket_table[i].state = OFFLINE;
						socket_table[i].index = -1;
						socket_table[i].lastActivity = clock();
						
    				}else if( clock() > socket_table[i].lastActivity+AFK_TIME){
    					printf("Put AFK: Client %d\n",socket_table[i].index);
					socket_table[i].state = AFK;
    				}
    			}
    		}
	}
	else if(signo==SIGINT){	/* handler of SIGNAL Run Interruption*/
		for(int i=0;i<connections;i++){    /* Close sockets*/
    			if(socket_table[i].state!=OFFLINE){ /* Verify IF ative*/
    				
				printf("Close connection: Client %d\n",socket_table[i].index);
				
				shutdown(socket_table[i].socket,SHUT_RD);
				shutdown(socket_table[i].socket,SHUT_WR);
				shutdown(socket_table[i].socket,SHUT_RDWR);
				
				//clear the socket
				socket_table[i].socket = -1;
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
	addr.sin_addr.s_addr = INADDR_ANY;                  //       IP 0.0.0.0 (/* any interface */)
	

	if ( bind(listen_sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
		panic("bind");

	/*--- make into listener with 10 slots ---*/
	if ( listen(listen_sd, MAX_CLIENT_NUM) != 0 )
		panic("listen")

	/*--- begin waiting for connections ---*/
	else
	{	int st=0;
		pthread_t scanTerminalThread;
		pthread_create(&scanTerminalThread, 0, taskScanTerminal,&st);       /* start thread */
		pthread_detach(scanTerminalThread);                    /* don't track it */
	
		signal(SIGALRM,checkState); // set signal (alarm)
		struct itimerval itv;
		itv.it_interval.tv_sec = 5;
		itv.it_interval.tv_usec = 0;
		itv.it_value.tv_sec = 5;
		itv.it_value.tv_usec = 0;
		setitimer (ITIMER_REAL, &itv, NULL); /* send signal to process for every 5 seconds*/
		
		//MAIN LOOP
		while (1)                         /* process all incoming clients */
		{
			int n = sizeof(addr);
			int clientsd = accept(listen_sd, (struct sockaddr*)&addr, &n);     /* accept connection */
			if(clientsd!=-1)	//client found
			{
				/*Create socket*/

				printf("New connection: Client %d\n",connections);				
				socket_table[connections].socket=clientsd;
				socket_table[connections].state = ONLINE;		/*means connection opened*/
				socket_table[connections].index=connections;
				socket_table[connections].lastActivity=clock(); /*End time 1 min*/
				client_socket_info_t *info;
				info = malloc (sizeof (client_socket_info_t)); /* allocate memory for socket information */
				info = &socket_table[connections];
				
				pthread_t broadcastThread;
				pthread_create(&broadcastThread, 0, taskBroadcast, info);       /* start thread of braodcast */
				pthread_detach(broadcastThread);                      /* don't track it */

				connections++;
			}
			
		}
	}
}
