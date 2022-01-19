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

//#include <sys/semaphore.h>

#include "airSensor.h"
#include "WaterTempsensor.h"
#include "LDRsensor.h"

#include "buffer.h"

#include "heater.h"
#include "stepmotor.h"
#include "light.h"
#include "waterpump.h"



#ifndef NULL
#define NULL 0
#endif


void setPrio(unsigned int priority) {
	struct sched_param Priority_Param; //struct to set priority
	int policy = SCHED_FIFO; //kind of policy desired, either SCHED_FIFO or SCHED_RR, otherwise Linux uses SCHED_OTHER
	pthread_t self_id = pthread_self(); // I ask for the tid..
	Priority_Param.sched_priority = priority; //.. set the priority (the higher the sooner it is executed, min at 1, max at 99)..
	if (pthread_setschedparam(self_id, policy, &Priority_Param) != 0) //.. and set the scheduling options.
	{
		printf("Error Set Sched\n");
		perror("");
		exit(1);
	}
}

/*Semaphores*/
sem_t semaphoreAirTemperature;
sem_t semaphoreAirHumidity;
sem_t semaphoreWaterTemperature;
sem_t semaphoreLightLevel;
sem_t semaphorePhotoBuffer;
sem_t semaphoreProcessedPhotoBuffer;

/*buffers*/
Buffer<float> airTemperatureBuffer;
Buffer<float> airHumidityBuffer;
Buffer<float> waterTemperatureBuffer;
Buffer<float> lightLevelBuffer;
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

/*Critical error routine*/
void panic(char* msg);
#define panic(m)		{perror(m); abort();}

/*classes*/
WaterPump waterPump;
StepMotor stepMotor;
Light light;
Heater heater;

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

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

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskReadSensors(void*) {

	int airTemperature, airHumidity;

	//read air temperature
	AirSensor airsensor;
	airsensor.init(1, 0x40);
	char buf[10];
	buf[0]=0xE3;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	airTemperature=buf[0];

	//read air humidty
	char buf[10];
	buf[0]=0xE5;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	airHumidity=buf[0];

	sem_wait(&semaphoreAirTemperature);
		if (!airTemperatureBuffer.add(airTemperature)) {/*buffer full*/ }
	sem_post(&semaphoreAirTemperature);

	sem_wait(&semaphoreAirHumidity);
		if (!airHumidityBuffer.add(airHumidity)) {/*buffer full*/ }
	sem_post(&semaphoreAirHumidity);

	int waterTemperature;

	//read water temperature

	sem_wait(&semaphoreWaterTemperature);
		if (!waterTemperatureBuffer.add(waterTemperature)) {/*buffer full*/ }
	sem_post(&semaphoreWaterTemperature);

	int lightLevel;

	//read light level

	sem_wait(&semaphoreLightLevel);
		if (!lightLevelBuffer.add(lightLevel)) {/*buffer full*/ }
	sem_post(&semaphoreLightLevel);
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskTakePhoto(void*) {
	setPrio(1);
	return NULL;
}
void* taskProcessPhoto(void*) {
	setPrio(2);
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskSendData(void*) {
	setPrio(4);
	int airTemperature, airHumidity;

	sem_wait(&semaphoreAirTemperature);
		while (!airTemperatureBuffer.check(&airTemperature)) {/*buffer empty*/ }
	sem_post(&semaphoreAirTemperature);

	sem_wait(&semaphoreAirHumidity);
		while (!airHumidityBuffer.check(&airHumidity)) {/*buffer empty*/ }
	sem_post(&semaphoreAirHumidity);

	int waterTemperature;

	sem_wait(&semaphoreWaterTemperature);
		while (!waterTemperatureBuffer.check(&waterTemperature)) {/*buffer empty*/ }
	sem_post(&semaphoreWaterTemperature);

	int lightLevel;

	sem_wait(&semaphoreLightLevel);
		while (!lightLevelBuffer.check(&lightLevel)) {/*buffer empty*/ }
	sem_post(&semaphoreLightLevel);

	//insert in the database
	return NULL;
}

void* taskSendPhoto(void*) {
	printf("Thread ON\n");
	setPrio(3);
	//photo struct

	sem_wait(&semaphoreProcessedPhotoBuffer);
		//while (!lightLevelBuffer.remove(/*//photo struct*/)) {/*buffer empty*/ }
	sem_post(&semaphoreProcessedPhotoBuffer);

	//insert in the database
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskProcessAirTemperature(void*) {
	setPrio(5);
	while(1){
		int airTemperature, tHeaterPower;
		float result;

		sem_wait(&semaphoreAirTemperature);
			while (!airTemperatureBuffer.remove(&airTemperature)) {/*buffer empty*/ }
		sem_post(&semaphoreAirTemperature);
	
		//convert
		float Temp_Code = airTemperature;
		result = (175.72 * Temp_Code / 65536) + 46.85;
	
		if(result<20)
			tHeaterPower=100;
		else if(result>20 && result<30)
			tHeaterPower=(30-result)*10;
		else
			tHeaterPower=0;
	
	
		pthread_mutex_lock(&mutexTargetHeaterPower);
			targetHeaterPower = tHeaterPower;
		pthread_mutex_unlock(&mutexTargetHeaterPower);
		sleep(4);
	}
	return NULL;
}

void* taskProcessAirHumidity(void*) {
	setPrio(5);
	while(1){
		int airHumidity, tMotorPosition;
		float result;
	
		sem_wait(&semaphoreAirHumidity);
			while (!airTemperatureBuffer.remove(&airHumidity)) {/*buffer empty*/ }
		sem_post(&semaphoreAirHumidity);
	
		//convert
		float RH_Code = airHumidity;
		result = (125 * RH_Code / 65536) + 6;
	
		if(result<40)
			tMotorPosition=0;
		else
			tMotorPosition=90;
	
		pthread_mutex_lock(&mutexTargetMotorPosition);
			targetMotorPosition = tMotorPosition;
		pthread_mutex_unlock(&mutexTargetMotorPosition);
		sleep(4);
	}
	return NULL;
}

void* taskProcessLightLevel(void*) {
	setPrio(5);
	while(1){
		int lightLevel, tLightPower, wPumpState;
		float result;
	
		sem_wait(&semaphoreLightLevel);
			while (!airTemperatureBuffer.remove(&lightLevel)) {/*buffer empty*/ }
		sem_post(&semaphoreLightLevel);
	
		//convert


		if(result<40){
			wPumpState=100;
			tLightPower=100
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

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskActuateHeater(void*) {
	setPrio(7);
	while(1){
		pthread_mutex_lock(&mutexTargetHeaterPower);
		int tHeaterPower=targetHeaterPower;
		pthread_mutex_unlock(&mutexTargetHeaterPower);
		//
		if(tHeaterPower)
			heater.turnOn();
		else
			heater.turnOff();
		sleep(4);
	}
	return NULL;
}

void* taskActuateWindow(void*) {
	setPrio(7);
	while(1){
		pthread_mutex_lock(&mutexTargetMotorPosition);
		int tMotorPosition=targetMotorPosition;
		pthread_mutex_unlock(&mutexTargetMotorPosition);
		//
		stepMotor.rotateTo(tMotorPosition);
		sleep(4);
	}
	return NULL;
}

void* taskActuateLight(void*) {
	setPrio(7);
	while(1){
		pthread_mutex_lock(&mutexTargetLightPower);
		int tLightPower=targetLightPower;
		pthread_mutex_unlock(&mutexTargetLightPower);
		//
		if(tLightPower)
			light.turnOn();
		else
			light.turnOff();
		sleep(4);
	}
	return NULL;
}

void* taskActuateWaterPump(void*) {
	setPrio(7);
	while(1){
		pthread_mutex_lock(&mutexWaterPumpState);
		int wPumpState=waterPumpState;
		pthread_mutex_unlock(&mutexWaterPumpState);
		//
		if(wPumpState)
			waterPump.turnOn();
		else
			waterPump.turnOff();
		sleep(4);
	}
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskCheckWifiDataReception(void*) {return NULL;}
void* taskSetAirTemperature(void) {
	setPrio(6);
	return NULL;
}
void* taskSetAirHumidity(void) {
	setPrio(6);
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
int main(int count, char* args[])
{
	//MAIN SETUP
	waterPump.init(12);
	stepMotor.init(6, 13, 19, 26, 0, 10000, 0);
	light.init(5);
	heater.init(25);

	//declare semaphores
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
	if (sem_init(&semaphorePhotoBuffer, 0, 1) != 0)
	{
		// Error: initialization failed
	}
	if (sem_init(&semaphoreProcessedPhotoBuffer, 0, 1) != 0)
	{
		// Error: initialization failed
	}
	

	pthread_t threadTakePhoto, threadProcessPhoto;
	pthread_create(&threadTakePhoto, 0, taskTakePhoto, NULL);
	pthread_create(&threadProcessPhoto, 0, taskProcessPhoto, NULL);
	pthread_detach(threadTakePhoto);
	pthread_detach(threadProcessPhoto);

	pthread_t threadSendData, threadSendPhoto;
	pthread_create(&threadSendData, 0, taskSendData, NULL);
	pthread_create(&threadSendPhoto, 0, taskSendPhoto, NULL);
	pthread_detach(threadSendData);
	pthread_detach(threadSendPhoto);

	pthread_t threadProcessAirTemperature, threadProcessAirHumidity, threadProcessLightLevel;
	pthread_create(&threadProcessAirTemperature, 0, taskProcessAirTemperature, NULL);
	pthread_create(&threadProcessAirHumidity, 0, taskProcessAirHumidity, NULL);
	pthread_create(&threadProcessLightLevel, 0, taskProcessLightLevel, NULL);
	pthread_detach(threadProcessAirTemperature);
	pthread_detach(threadProcessAirHumidity);
	pthread_detach(threadProcessLightLevel);

	pthread_t threadActuateHeater, threadActuateWindow, threadActuateLight, threadActuateWaterPump;
	pthread_create(&threadActuateHeater, 0, taskActuateHeater, NULL);
	pthread_create(&threadActuateWindow, 0, taskActuateWindow, NULL);
	pthread_create(&threadActuateLight, 0, taskActuateLight, NULL);
	pthread_create(&threadActuateWaterPump, 0, taskActuateWaterPump, NULL);
	pthread_detach(threadActuateHeater);
	pthread_detach(threadActuateWindow);
	pthread_detach(threadActuateLight);
	pthread_detach(threadActuateWaterPump);

	pthread_t threadCheckWifiDataReception;
	pthread_create(&threadCheckWifiDataReception, 0, taskCheckWifiDataReception, NULL);
	pthread_detach(threadCheckWifiDataReception);

	while (1)
	{
		taskReadSensors(NULL);
	}

	int pid = fork();//new daemon
	if (pid < 0) {//fail
		syslog(LOG_ERR, "%s\n", "fork");
		exit(EXIT_FAILURE);
	}
	if (pid > 0) {//success
		printf("Deamon PID: %d\n", pid);
		exit(EXIT_SUCCESS);
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

	//MAIN LOOP
	while (1)
	{
		taskReadSensors(NULL);
	}
}
