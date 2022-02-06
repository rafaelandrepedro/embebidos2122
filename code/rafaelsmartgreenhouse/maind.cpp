#include "main2.h"

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
#include <sstream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <mqueue.h>   /* mq_* functions */

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/queue0000"
/* max length of a message (just for this process) */
#define MAX_MSG_LEN     10000

#include "airsensor.h"
#include "LDRsensor.h"
#include "WaterTempsensor.h"



#ifndef NULL
#define NULL 0
#endif

/*Critical error routine*/
void panic(char* msg);
#define panic(m)		{perror(m); abort();}

/*classes*/
LDRsensor ldrsensor;
WaterTempsensor watertempsensor;
Airsensor airsensor;

/*message queue*/
mqd_t msgq_id;

//?????????????????????????????????????????????????????????????????????????????????????????????
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

//?????????????????????????????????????????????????????????????????????????????????????????????


//?????????????????????????????????????????????????????????????????????????????????????????????
/**
 * @brief reads the sensors and stores the values in the buffers
 *
 * @param[out] the values read
 * @return void
 */
void taskReadSensors(int values[4]) {

	int waterTemperature;
	
	//read water temperature
	waterTemperature=watertempsensor.adcGetValue(1);

	int lightLevel;

	//read light level
	lightLevel=ldrsensor.adcGetValue(0);

	int airTemperature, airHumidity;
	char buf[10];
	//read air temperature
	buf[0]=0xE3;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	airTemperature=buf[0];

	//read air humidty
	buf[0]=0xE5;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	airHumidity=buf[0];
	
	values[0]=waterTemperature;
	values[1]=lightLevel;
	values[2]=airTemperature;
	values[3]=airHumidity;
	
	return;
}

//?????????????????????????????????????????????????????????????????????????????????????????????


int main(int count, char* args[])
{
	
	
	//init the system classes
	airsensor.init(1, 0x40);

	msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);
	if (msgq_id == (mqd_t)-1) {
		perror("In mq_open()");
		exit(1);
	}
	
	
	int pid = fork();//new daemon
	
	if (pid < 0) {//fail
		syslog(LOG_ERR, "%s\n", "fork");
		exit(EXIT_FAILURE);
	}
	
	if (pid > 0) {//success
		printf("Deamon PID: %d\n", pid);	
		exit(EXIT_SUCCESS); // parent process (exit)
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

	int msgprio = 1;

	//MAIN LOOP
	while (1)
	{
		sleep(4);
		int values[4];
		taskReadSensors(values);
		std::string msg = std::to_string(values[0]) + std::to_string(values[1]) + std::to_string(values[2]) + std::to_string(values[3]);
		
		// sending the message      --  mq_send() 
		mq_send(msgq_id, std::to_string(values[0]).c_str(), strlen(std::to_string(values[0]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[1]).c_str(), strlen(std::to_string(values[1]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[2]).c_str(), strlen(std::to_string(values[2]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[3]).c_str(), strlen(std::to_string(values[3]).c_str())+1, msgprio);

		
	}
}
