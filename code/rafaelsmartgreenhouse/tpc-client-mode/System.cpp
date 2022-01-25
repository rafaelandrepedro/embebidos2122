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


void panic(char* msg);
#define panic(m)		{perror(m); abort();}


//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�
void* periodicTask1(void* arg){

}

void* task1(void* arg) {

}

void* task2(void* arg) {

}

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�
int main(int count, char* args[])
{
	//MAIN SETUP
	
	//###create thread###
		int arguments1 = 0;
		pthread_t thread1;
		pthread_create(&thread1, 0, task1, &arguments1);       /* start thread */
		pthread_detach(thread1);                    /* don't track it */

	//###create periodic task###
		signal(SIGALRM, periodicTask1); // set signal (alarm)
		struct itimerval itv1;
		itv1.it_interval.tv_sec = itv1.it_value.tv_sec = 5;
		itv1.it_interval.tv_usec = itv1.it_value.tv_usec = 0;
		setitimer(ITIMER_REAL, &itv1, NULL); /* send signal to process for every 5 seconds*/

	//###create daemon###
		pid = fork();//new daemon
		if (pid < 0) {//fail
			syslog(LOG_ERR, "%s\n", "fork");
			exit(EXIT_FAILURE);
		}
		if (pid > 0) {//success
			printf("Deamon PID: %d\n", pid);
			exit(EXIT_SUCCESS);
		}
		sid = setsid();//new session
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
		while (1) {
			//process
		}

	//MAIN LOOP
		while (1)                         /* process all incoming clients */
		{

			int arguments2 = 0;
			pthread_t thread2;
			pthread_create(&thread2, 0, task2, &arguments2);       /* start thread */
			pthread_detach(thread2);                    /* don't track it */

		}
}
