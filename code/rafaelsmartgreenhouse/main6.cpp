#include <cmath>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <resolv.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>   /* mq_* functions */

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/myqueue12345890"
/* max length of a message (just for this process) */
#define MAX_MSG_LEN     10000


//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�
/**
 * @brief signal handler to handle the hangup and terminate signals
 *
 * @param[in] sig the signal recieved
 * @return void
 */
void signal_handler(int sig) {
	switch(sig) {
		case SIGHUP:
			syslog(LOG_INFO,"Hangup signal catched");
			break;
		case SIGTERM:
			syslog(LOG_INFO,"Terminate signal catched");
			exit(0);
			break;
	}
}

int main(int count, char* args[])
{
	
	mqd_t msgq_id;	
		
		msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, NULL);
		if (msgq_id == (mqd_t)-1) {
			perror("In mq_open()1");
			exit(1);
		}
	
	int pid = fork();//new daemon
	
	if (pid < 0) {//fail
		syslog(LOG_ERR, "%s\n", "fork");
		exit(EXIT_FAILURE);
	}
	
	if (pid > 0) {//success
		printf("Deamon PID: %d\n", pid);
		
			
		while(1){
			sleep(1);
			
			
		unsigned int msgprio = 1;
		char msgcontent[MAX_MSG_LEN];
		    
		
		
		
			
			//mqd_t msgq_id;
			    //char msgcontent[MAX_MSG_LEN];
			    int msgsz;
			    unsigned int sender;

			    /* getting a message */
			    msgsz = mq_receive(msgq_id, msgcontent, MAX_MSG_LEN+1, &sender);
			    printf("Received message (%d bytes) from %d: %s\n", msgsz, sender, msgcontent);
			    /* closing the queue    --  mq_close() */
			    //mq_close(msgq_id);
			    /* ...and finally unlink it. After unlink message queue is removed from system. */
		}
		pthread_exit(NULL);
	}
	
	int sid = setsid();//new session
	if (sid < 0) {//fail
		syslog(LOG_ERR, "%s\n", "setsid");
		exit(EXIT_FAILURE);
	}
	
	//change dir to root
	if (chdir("/") < 0) {//fail
		syslog(LOG_ERR, "%s\n", "chdir");
		exit(EXIT_FAILURE);
	}
	
	umask(0);//mask
	close(STDIN_FILENO);  // close standard input file descriptor
	close(STDOUT_FILENO); // close standard output file descriptor
	close(STDERR_FILENO); // close standard error file descriptor
	signal(SIGHUP, signal_handler); /* catch hangup signal */
	signal(SIGTERM, signal_handler); /* catch kill signal */

	unsigned int msgprio = 1;
		char msgcontent[MAX_MSG_LEN];
		

		
	
	//MAIN LOOP
	while (1)
	{
		sleep(1);		
		
		// sending the message      --  mq_send() 
		mq_send(msgq_id, "hello", strlen("hello")+1, msgprio);
		printf("%d %s %d %d",msgq_id, "hello", strlen("hello")+1, msgprio);
		// closing the queue        -- mq_close() 
		//mq_close(msgq_id); 
		
	}
}
