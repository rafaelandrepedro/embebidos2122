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

#include "airsensor.h"
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


void initPrio(int* thread_policy, pthread_attr_t* thread_attr, struct sched_param* thread_param){
	pthread_attr_init (thread_attr);
	pthread_attr_getschedpolicy (thread_attr, thread_policy);
	pthread_attr_getschedparam (thread_attr, thread_param);
}

void setPrio(unsigned int priority, pthread_attr_t* thread_attr, struct sched_param* thread_param) {
	thread_param->sched_priority = priority;
	
	pthread_attr_setschedparam (thread_attr, thread_param);
	pthread_attr_setinheritsched (thread_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy (thread_attr, SCHED_RR);
}

/*Semaphores*/
sem_t semaphoreAirTemperature;
sem_t semaphoreAirHumidity;
sem_t semaphoreWaterTemperature;
sem_t semaphoreLightLevel;
sem_t semaphorePhotoBuffer;
sem_t semaphoreProcessedPhotoBuffer;

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
	while (1){

	int airTemperature, airHumidity;
	char buf[10];
	//read air temperature
	buf[0]=0xE3;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	airTemperature=buf[0];
	printf("Temp: %d\n", buf[0]);

	//read air humidty
	buf[0]=0xE5;
	airsensor.cwrite(buf);
	airsensor.cread(buf);
	airHumidity=buf[0];
	printf("Humidity: %d\n", buf[0]);

	sem_wait(&semaphoreAirTemperature);
		if (!airTemperatureBuffer.add(airTemperature)) {/*buffer full*/ }
	sem_post(&semaphoreAirTemperature);

	sem_wait(&semaphoreAirHumidity);
		if (!airHumidityBuffer.add(airHumidity)) {/*buffer full*/ }
	sem_post(&semaphoreAirHumidity);

	int waterTemperature;

	//read water temperature
	waterTemperature=watertempsensor.adcGetValue(1);

	sem_wait(&semaphoreWaterTemperature);
		if (!waterTemperatureBuffer.add(waterTemperature)) {/*buffer full*/ }
	sem_post(&semaphoreWaterTemperature);

	int lightLevel;

	//read light level
	lightLevel=ldrsensor.adcGetValue(0);

	sem_wait(&semaphoreLightLevel);
		if (!lightLevelBuffer.add(lightLevel)) {/*buffer full*/ }
	sem_post(&semaphoreLightLevel);
	sleep(5);
	}
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskTakePhoto(void*) {
	return NULL;
}
void* taskProcessPhoto(void*) {
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

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
	return NULL;
}

void* taskSendPhoto(void*) {
	//photo struct

	sem_wait(&semaphoreProcessedPhotoBuffer);
		//while (!lightLevelBuffer.remove(/*//photo struct*/)) {/*buffer empty*/ }
	sem_post(&semaphoreProcessedPhotoBuffer);

	//insert in the database
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskProcessAirTemperature(void*) {
	while(1){
		int airTemperature, tHeaterPower;
		float result;

		sem_wait(&semaphoreAirTemperature);
			if (!airTemperatureBuffer.remove(&airTemperature)) {/*buffer empty*/ }
		sem_post(&semaphoreAirTemperature);
		printf("Temp lida: %d\n", airTemperature);
		//convert
		float Temp_Code = airTemperature;
		result = (175.72 * Temp_Code / 256) - 46.85;
		printf("Temp processada: %d\n", (int)Temp_Code);
	
		if((int)result<refTemperature)
			tHeaterPower=100;
		else if((int)result>refTemperature && (int)result<refTemperature+10)
			tHeaterPower=(refTemperature+10-(int)result)*10;
		else
			tHeaterPower=0;
		printf("result: %d\n",(int)result);
		printf("tHeaterPower: %d\n",tHeaterPower);
	
		pthread_mutex_lock(&mutexTargetHeaterPower);
			targetHeaterPower = tHeaterPower;
		pthread_mutex_unlock(&mutexTargetHeaterPower);		
		
		sleep(4);
	}
	return NULL;
}

void* taskProcessAirHumidity(void*) {
	while(1){
		int airHumidity, tMotorPosition;
		float result;
	
		sem_wait(&semaphoreAirHumidity);
			if (!airHumidityBuffer.remove(&airHumidity)) {/*buffer empty*/ }
		sem_post(&semaphoreAirHumidity);
	
		//convert
		float RH_Code = airHumidity;
		result = (125 * RH_Code / 256) + 6;
	
		if(result<refHumidity)
			tMotorPosition=0;
		else
			tMotorPosition=256;
		/*
		pthread_mutex_lock(&mutexTargetMotorPosition);
			targetMotorPosition = tMotorPosition;
		pthread_mutex_unlock(&mutexTargetMotorPosition);
		*/
		sleep(4);
	}
	return NULL;
}

void* taskProcessLightLevel(void*) {
	while(1){
		int lightLevel, tLightPower, wPumpState;
		float result;
	
		sem_wait(&semaphoreLightLevel);
			if (!lightLevelBuffer.remove(&lightLevel)) {/*buffer empty*/ }
		sem_post(&semaphoreLightLevel);
	
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

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskActuateHeater(void*) {
	while(1){
		pthread_mutex_lock(&mutexTargetHeaterPower);
		int tHeaterPower=targetHeaterPower;
		pthread_mutex_unlock(&mutexTargetHeaterPower);
		//
		printf("Actuate heater at %d%\n", tHeaterPower);
		heater.actuate(tHeaterPower);
		sleep(4);
	}
	return NULL;
}

void* taskActuateWindow(void*) {
	while(1){
		pthread_mutex_lock(&mutexTargetMotorPosition);
		int tMotorPosition=targetMotorPosition;
		pthread_mutex_unlock(&mutexTargetMotorPosition);
		//
		printf("Actuate motor at %d%\n", tMotorPosition);
		stepMotor.rotateTo(tMotorPosition);
		sleep(4);
	}
	return NULL;
}

void* taskActuateLight(void*) {
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
void* taskSetAirTemperature(void*) {
	return NULL;
}
void* taskSetAirHumidity(void*) {
	return NULL;
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
int main(int count, char* args[])
{
	//MAIN SETUP
	waterPump.init(25);//GPIO25
	stepMotor.init(13, 6, 5, 26, 0, 5000, 0);//GPIO13,6,5,26
	light.init(23);//GPIO23
	heater.init(24);//GPIO24

	airsensor.init(1, 0x40);

	refTemperature=20;
	refHumidity=40;

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
	
	int thread_policy;
	pthread_attr_t thread_attr;
	struct sched_param thread_param;
	
	initPrio(&thread_policy, &thread_attr, &thread_param);
	
	

	pthread_t threadTakePhoto, threadProcessPhoto;
	setPrio(7, &thread_attr, &thread_param);
	pthread_create(&threadTakePhoto, &thread_attr, taskTakePhoto, NULL);
	setPrio(6, &thread_attr, &thread_param);
	pthread_create(&threadProcessPhoto, &thread_attr, taskProcessPhoto, NULL);
	pthread_detach(threadTakePhoto);
	pthread_detach(threadProcessPhoto);

	pthread_t threadSendData, threadSendPhoto;
	setPrio(4, &thread_attr, &thread_param);
	pthread_create(&threadSendData, &thread_attr, taskSendData, NULL);
	setPrio(5, &thread_attr, &thread_param);
	pthread_create(&threadSendPhoto, &thread_attr, taskSendPhoto, NULL);
	pthread_detach(threadSendData);
	pthread_detach(threadSendPhoto);

	pthread_t threadProcessAirTemperature, threadProcessAirHumidity, threadProcessLightLevel;
	setPrio(3, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirTemperature, &thread_attr, taskProcessAirTemperature, NULL);
	setPrio(3, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirHumidity, &thread_attr, taskProcessAirHumidity, NULL);
	setPrio(3, &thread_attr, &thread_param);	
	pthread_create(&threadProcessLightLevel, &thread_attr, taskProcessLightLevel, NULL);
	pthread_detach(threadProcessAirTemperature);
	pthread_detach(threadProcessAirHumidity);
	pthread_detach(threadProcessLightLevel);

	pthread_t threadActuateHeater, threadActuateWindow, threadActuateLight, threadActuateWaterPump;
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateHeater, &thread_attr, taskActuateHeater, NULL);
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateWindow, &thread_attr, taskActuateWindow, NULL);
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateLight, &thread_attr, taskActuateLight, NULL);
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateWaterPump, &thread_attr, taskActuateWaterPump, NULL);
	pthread_detach(threadActuateHeater);
	pthread_detach(threadActuateWindow);
	pthread_detach(threadActuateLight);
	pthread_detach(threadActuateWaterPump);

	pthread_t threadCheckWifiDataReception;
	setPrio(6, &thread_attr, &thread_param);
	pthread_create(&threadCheckWifiDataReception, &thread_attr, taskCheckWifiDataReception, NULL);
	pthread_detach(threadCheckWifiDataReception);
	
	pthread_t threadSetAirTemperature, threadSetAirHumidity;
	setPrio(2, &thread_attr, &thread_param);
	pthread_create(&threadSetAirTemperature, &thread_attr, taskSetAirTemperature, NULL);
	setPrio(2, &thread_attr, &thread_param);
	pthread_create(&threadSetAirHumidity, &thread_attr, taskSetAirHumidity, NULL);
	pthread_detach(threadSetAirTemperature);
	pthread_detach(threadSetAirHumidity);

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
