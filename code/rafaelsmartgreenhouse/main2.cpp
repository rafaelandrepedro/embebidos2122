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
<<<<<<< HEAD
#include <mqueue.h>   /* mq_* functions */

/* name of the POSIX object referencing the queue */
#define MSGQOBJ_NAME    "/queue0000"
/* max length of a message (just for this process) */
#define MAX_MSG_LEN     10000
=======
>>>>>>> parent of e2915751 (.)

//#include <sys/semaphore.h>

#include "airsensor.h"
#include "LDRsensor.h"
#include "WaterTempsensor.h"


#include "buffer.h"

#include "heater.h"
#include "stepmotor.h"
#include "light.h"
#include "waterpump.h"



#ifndef NULL
#define NULL 0
#endif

/**
 * @brief initialization of the thread priority attributes
 *
 * @param[out] policy policy used by the scheduler
 * @param[out] thread_attr thread attribute struct
 * @param[out] sched_param list of parameters used by the scheduler
 * @return void
 */
void initPrio(int* thread_policy, pthread_attr_t* thread_attr, struct sched_param* thread_param){
	pthread_attr_init (thread_attr);
	pthread_attr_getschedpolicy (thread_attr, thread_policy);
	pthread_attr_getschedparam (thread_attr, thread_param);
}

/**
 * @brief set the priority of a thread
 *
 * @param[in] priority priority value (the higher the sooner it will run)
 * @param[in] thread_attr thread attribute struct
 * @param[in] sched_param list of parameters used by the scheduler
 * @return void
 */
void setPrio(unsigned int priority, pthread_attr_t* thread_attr, struct sched_param* thread_param) {
	thread_param->sched_priority = priority;
	
	pthread_attr_setinheritsched (thread_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy (thread_attr, SCHED_RR);
	pthread_attr_setschedparam (thread_attr, thread_param);
}

/*Semaphores*/
sem_t semaphoreAirTemperature;
sem_t semaphoreAirHumidity;
sem_t semaphoreWaterTemperature;
sem_t semaphoreLightLevel;

/*buffers*/
Buffer<int> airTemperatureBuffer;
Buffer<int> airHumidityBuffer;
Buffer<int> waterTemperatureBuffer;
Buffer<int> lightLevelBuffer;
//photoBuffer
//processedPhotoBuffer

/*Mutexes*/
pthread_mutex_t mutexTargetMotorPosition;
pthread_mutex_t mutexTargetHeaterPower;
pthread_mutex_t mutexTargetLightPower;
pthread_mutex_t mutexWaterPumpState;

/*Control variables*/
int targetMotorPosition;
int targetHeaterPower;
int targetLightPower;
int waterPumpState;

int refTemperature;
int refHumidity;

/*Critical error routine*/
void panic(char* msg);
#define panic(m)		{perror(m); abort();}

/*classes*/
LDRsensor ldrsensor;
WaterTempsensor watertempsensor;
Airsensor airsensor;
WaterPump waterPump;
StepMotor stepMotor;
Light light;
Heater heater;

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

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�
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

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�
/**
 * @brief takes a photo and stores in the rasp
 *
 * @return void
 */
void* taskTakePhoto(void*) {
<<<<<<< HEAD
	//system("raspistill -o [nome]");
=======
>>>>>>> parent of e2915751 (.)
	return NULL;
}

/**
 * @brief moves the photo into a specific directory
 *
 * @return void
 */
void* taskProcessPhoto(void*) {
	return NULL;
}

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�
/**
 * @brief stores the sensor values into the database
 *
 * @return void
 */
void* taskSendData(void*) {
	int airTemperature, airHumidity;

	sem_wait(&semaphoreAirTemperature);
		if (!airTemperatureBuffer.check(&airTemperature)) {/*buffer empty*/ }
	sem_post(&semaphoreAirTemperature);

	sem_wait(&semaphoreAirHumidity);
		if (!airHumidityBuffer.check(&airHumidity)) {/*buffer empty*/ }
	sem_post(&semaphoreAirHumidity);

	int waterTemperature;

	sem_wait(&semaphoreWaterTemperature);
		if (!waterTemperatureBuffer.check(&waterTemperature)) {/*buffer empty*/ }
	sem_post(&semaphoreWaterTemperature);

	int lightLevel;

	sem_wait(&semaphoreLightLevel);
		if (!lightLevelBuffer.check(&lightLevel)) {/*buffer empty*/ }
	sem_post(&semaphoreLightLevel);

	//insert in the database
	//db.quarry("INSERT INTO COMPANY (ID,AIRTEMPERATURE,AIRHUMIDITY,LIGHTLEVEL,WATERTEMPERATURE) VALUES (1, 20, 40, 50, 20 ); ");
	return NULL;
}

/**
 * @brief sends the photo to the mobile app
 *
 * @return void
 */
void* taskSendPhoto(void*) {
	//photo struct
	return NULL;
}

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�

/**
 * @brief converts the air temperature from the buffer and compares with the reference temperature, then sends a signal to the actuation variable of the heater
 *
 * @return void
 */
void* taskProcessAirTemperature(void*) {
	while(1){
		int airTemperature, tHeaterPower;
		float result;

		sem_wait(&semaphoreAirTemperature);
			if (!airTemperatureBuffer.remove(&airTemperature)) {/*buffer empty*/ }
		sem_post(&semaphoreAirTemperature);
		//convert
		float Temp_Code = (float)airTemperature;
		result = ((float)175.72 * (float)Temp_Code / (float)256) - (float)46.85;
	
		if((int)result<=refTemperature)
			tHeaterPower=100;
		else if((int)result>refTemperature && (int)result<refTemperature+10)
			tHeaterPower=(refTemperature+10-(int)result)*10;
		else
			tHeaterPower=0;
			
		printf("\033[1;33m");
		printf("Temp processada: %f\n", result);
		printf("\033[0m");
	
		pthread_mutex_lock(&mutexTargetHeaterPower);
			targetHeaterPower = tHeaterPower;
		pthread_mutex_unlock(&mutexTargetHeaterPower);		
		
		sleep(4);
	}
	return NULL;
}

/**
 * @brief converts the air humidity from the buffer and compares with the reference humidity, then sends a signal to the actuation variable of the stepmotor
 *
 * @return void
 */
void* taskProcessAirHumidity(void*) {
	while(1){
		int airHumidity, tMotorPosition;
		float result;
	
		sem_wait(&semaphoreAirHumidity);
			if (!airHumidityBuffer.remove(&airHumidity)) {/*buffer empty*/ }
		sem_post(&semaphoreAirHumidity);
		//convert
		float RH_Code = (float)airHumidity;
		result = ((float)125 * (float)RH_Code / (float)256) + (float)6;
	
		if((int)result<=refHumidity)
			tMotorPosition=256;
		else if((int)result>refHumidity && (int)result<refHumidity+10)
			tMotorPosition=(refHumidity+10-(int)result)*25.6;
		else
			tMotorPosition=0;
			
		printf("\033[1;33m");
		printf("Hum processada: %f\n", result);
		printf("\033[0m");
		
		
		pthread_mutex_lock(&mutexTargetMotorPosition);
			targetMotorPosition = tMotorPosition;
		pthread_mutex_unlock(&mutexTargetMotorPosition);
		
		sleep(4);
	}
	return NULL;
}

/**
 * @brief takes the light level from the buffer and compares with a reference light value, checks the sleeping time and then sends a signal to the actuation variables of the light and the water pump
 *
 * @return void
 */
void* taskProcessLightLevel(void*) {
	while(1){
		int lightLevel, tLightPower, wPumpState;
		float result;
	
		sem_wait(&semaphoreLightLevel);
			if (!lightLevelBuffer.remove(&lightLevel)) {/*buffer empty*/ }
		sem_post(&semaphoreLightLevel);
		
		printf("\033[1;33m");
		printf("Lux lida: %d\n", lightLevel);
		printf("\033[0m");
		
		//convert


		if(result<40){
			wPumpState=100;
			tLightPower=100;
		}
		else{
			wPumpState=100; //rever sleep time
			tLightPower=0;
		}
	
		pthread_mutex_lock(&mutexTargetLightPower);
			targetLightPower = tLightPower;
		pthread_mutex_unlock(&mutexTargetLightPower);
	
		pthread_mutex_lock(&mutexTargetLightPower);
			waterPumpState = wPumpState;
		pthread_mutex_unlock(&mutexTargetLightPower);
		sleep(4);
	}
	return NULL;
}

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�

/**
 * @brief checks the heater actuation variable and sets the heater to the target temperature
 *
 * @return void
 */
void* taskActuateHeater(void*) {
	while(1){
		pthread_mutex_lock(&mutexTargetHeaterPower);
		int tHeaterPower=targetHeaterPower;
		pthread_mutex_unlock(&mutexTargetHeaterPower);
		//
		printf("\033[1;31m");
		printf("Set heater to %d\n", tHeaterPower);
		printf("\033[0m");
		
		for(int i=1;i<4;i++)
			if(tHeaterPower)
				heater.actuate(tHeaterPower);
			else
				sleep(1);
	}
	return NULL;
}

/**
 * @brief checks the stepmotor actuation variable and sets the stepmotor angle to the target position
 *
 * @return void
 */
void* taskActuateWindow(void*) {
	while(1){
		pthread_mutex_lock(&mutexTargetMotorPosition);
		int tMotorPosition=targetMotorPosition;
		pthread_mutex_unlock(&mutexTargetMotorPosition);
		//
		printf("\033[1;31m");
		printf("Rotate motor to %d degrees\n", tMotorPosition);
		printf("\033[0m");
		
		stepMotor.rotateTo(tMotorPosition);
		sleep(4);
	}
	return NULL;
}


/**
 * @brief checks the light actuation variable and sets the LED tape ON or OFF accordingly 
 *
 * @return void
 */
void* taskActuateLight(void*) {
	while(1){
		pthread_mutex_lock(&mutexTargetLightPower);
		int tLightPower=targetLightPower;
		pthread_mutex_unlock(&mutexTargetLightPower);
		//
		printf("\033[1;31m");
		printf("Set light to %d\n", tLightPower);
		printf("\033[0m");
		
		if(tLightPower)
			light.turnOn();
		else
			light.turnOff();
		sleep(4);
	}
	return NULL;
}

/**
 * @brief checks the water pump actuation variable and sets the water pump ON or OFF accordingly 
 *
 * @return void
 */
void* taskActuateWaterPump(void*) {
	while(1){
		pthread_mutex_lock(&mutexWaterPumpState);
		int wPumpState=waterPumpState;
		pthread_mutex_unlock(&mutexWaterPumpState);
		//
		printf("\033[1;31m");
		printf("Set water pump to %d\n", wPumpState);
		printf("\033[0m");
		
		if(wPumpState)
			waterPump.turnOn();
		else
			waterPump.turnOff();
		sleep(4);
	}
	return NULL;
}

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�

/**
 * @brief searches for a client and connects to it if found
 *
 * @return void
 */
void* taskCheckWifiDataReception(void*) {return NULL;}
/**
 * @brief sets the air temperature reference to a value given from the mobile app
 *
 * @return void
 */
void* taskSetAirTemperature(void*) {
	return NULL;
}
/**
 * @brief sets the air humidity reference to a value given from the mobile app
 *
 * @return void
 */
void* taskSetAirHumidity(void*) {
	return NULL;
}

//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�
int main(int count, char* args[])
{

	//MAIN SETUP
		
	//set text color to normal
	printf("\033[0;37m");
	
	
	//init the system classes
	waterPump.init();//GPIO25
	stepMotor.init(13, 6, 5, 26, 0, 5000, 0);//GPIO13,6,5,26
	light.init();//GPIO23
	heater.init();//GPIO24
	airsensor.init(1, 0x40);
	
	
	//set the startup reference control values
	refTemperature=20;
	refHumidity=100;
	
	
	//setup the database
	//Database db("database.db");
   	//db.quarry("CREATE TABLE REPORT("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "AIRTEMPERATURE           FLOAT," \
      "AIRHUMIDITY            FLOAT," \
      "LIGHTLEVEL        FLOAT," \
      "WATERTEMPERATURE         FLOAT);");

	//setup the system semaphores
	if (sem_init(&semaphoreAirTemperature, 0, 1) != 0)
	{
		// Error: initialization failed
	}
	if (sem_init(&semaphoreAirHumidity, 0, 1) != 0)
	{
		// Error: initialization failed
	}
	if (sem_init(&semaphoreWaterTemperature, 0, 1) != 0)
	{
		// Error: initialization failed
	}
	if (sem_init(&semaphoreLightLevel, 0, 1) != 0)
	{
		// Error: initialization failed
	}

	
	//setup the thread handlers
	int thread_policy;
	pthread_attr_t thread_attr;
	struct sched_param thread_param;
	
	initPrio(&thread_policy, &thread_attr, &thread_param);
	
	
	//threads: photo
	pthread_t threadTakePhoto, threadProcessPhoto;
	setPrio(7, &thread_attr, &thread_param);
	pthread_create(&threadTakePhoto, 0, taskTakePhoto, NULL);
	setPrio(6, &thread_attr, &thread_param);
	pthread_create(&threadProcessPhoto, 0, taskProcessPhoto, NULL);
	pthread_detach(threadTakePhoto);
	pthread_detach(threadProcessPhoto);

	//threads: send to mobile app
	pthread_t threadSendData, threadSendPhoto;
	setPrio(4, &thread_attr, &thread_param);
	pthread_create(&threadSendData, 0, taskSendData, NULL);
	setPrio(5, &thread_attr, &thread_param);
	pthread_create(&threadSendPhoto, 0, taskSendPhoto, NULL);
	pthread_detach(threadSendData);
	pthread_detach(threadSendPhoto);
	
	//threads: process raw values and send actuation controls
	pthread_t threadProcessAirTemperature, threadProcessAirHumidity, threadProcessLightLevel;
	setPrio(3, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirTemperature, 0, taskProcessAirTemperature, NULL);
	setPrio(3, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirHumidity, 0, taskProcessAirHumidity, NULL);
	setPrio(3, &thread_attr, &thread_param);	
	pthread_create(&threadProcessLightLevel, 0, taskProcessLightLevel, NULL);
	pthread_detach(threadProcessAirTemperature);
	pthread_detach(threadProcessAirHumidity);
	pthread_detach(threadProcessLightLevel);
	
	//threads: control the actuators
	pthread_t threadActuateHeater, threadActuateWindow, threadActuateLight, threadActuateWaterPump;
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateHeater, 0, taskActuateHeater, NULL);
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateWindow, 0, taskActuateWindow, NULL);
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateLight, 0, taskActuateLight, NULL);
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateWaterPump, 0, taskActuateWaterPump, NULL);
	pthread_detach(threadActuateHeater);
	pthread_detach(threadActuateWindow);
	pthread_detach(threadActuateLight);
	pthread_detach(threadActuateWaterPump);
	
	//threads: connect to mobile app
	pthread_t threadCheckWifiDataReception;
	setPrio(6, &thread_attr, &thread_param);
	pthread_create(&threadCheckWifiDataReception, 0, taskCheckWifiDataReception, NULL);
	pthread_detach(threadCheckWifiDataReception);
	
	//threads: set reference parameters
	pthread_t threadSetAirTemperature, threadSetAirHumidity;
	setPrio(2, &thread_attr, &thread_param);
	pthread_create(&threadSetAirTemperature, 0, taskSetAirTemperature, NULL);
	setPrio(2, &thread_attr, &thread_param);
	pthread_create(&threadSetAirHumidity, 0, taskSetAirHumidity, NULL);
	pthread_detach(threadSetAirTemperature);
	pthread_detach(threadSetAirHumidity);
	
	
	mqd_t msgq_id;	
		
	msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT | O_EXCL, S_IRWXU | S_IRWXG, NULL);
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
		while(1){
			sleep(5);
			//int values[4];
			//taskReadSensors(values);
		    
			int msgsz;
			unsigned int sender;
			char msg[MAX_MSG_LEN];
			std::string str;
			/* getting a message */
			msgsz = mq_receive(msgq_id, msg, MAX_MSG_LEN+1, &sender);
			
			sem_wait(&semaphoreWaterTemperature);
				if (!waterTemperatureBuffer.add(std::atoi(msg))) {/*buffer full*/ }
			sem_post(&semaphoreWaterTemperature);
			
			printf("\033[1;33m");
			printf("W temp lida: %d\n", std::atoi(msg));
			printf("\033[0m");
			
			msgsz = mq_receive(msgq_id, msg, MAX_MSG_LEN+1, &sender);
			
			sem_wait(&semaphoreLightLevel);
				if (!lightLevelBuffer.add(std::atoi(msg))) {/*buffer full*/ }
			sem_post(&semaphoreLightLevel);
			
			msgsz = mq_receive(msgq_id, msg, MAX_MSG_LEN+1, &sender);
			
			sem_wait(&semaphoreAirTemperature);
				if (!airTemperatureBuffer.add(std::atoi(msg))) {/*buffer full*/ }
			sem_post(&semaphoreAirTemperature);
			
			msgsz = mq_receive(msgq_id, msg, MAX_MSG_LEN+1, &sender);
			
			sem_wait(&semaphoreAirHumidity);
				if (!airHumidityBuffer.add(std::atoi(msg))) {/*buffer full*/ }
			sem_post(&semaphoreAirHumidity);
			
			
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

	int msgprio = 1;

	//MAIN LOOP
	while (1)
	{
		sleep(5);
		int values[4];
		taskReadSensors(values);
<<<<<<< HEAD
		std::string msg = std::to_string(values[0]) + std::to_string(values[1]) + std::to_string(values[2]) + std::to_string(values[3]);
		
		// sending the message      --  mq_send() 
		mq_send(msgq_id, std::to_string(values[0]).c_str(), strlen(std::to_string(values[0]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[1]).c_str(), strlen(std::to_string(values[1]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[2]).c_str(), strlen(std::to_string(values[2]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[3]).c_str(), strlen(std::to_string(values[3]).c_str())+1, msgprio);

		
=======
		sprintf(buf, "%8d,%8d,%8d,%8d\n", values[0], values[1], values[2], values[3]);
		write(fd, buf, 37);
>>>>>>> parent of e2915751 (.)
	}
}
