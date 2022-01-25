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


void initPrio(int* thread_policy, pthread_attr_t* thread_attr, struct sched_param* thread_param){/*
	pthread_attr_init (thread_attr);
	pthread_attr_getschedpolicy (thread_attr, thread_policy);
	pthread_attr_getschedparam (thread_attr, thread_param);*/
}

void setPrio(unsigned int priority, pthread_attr_t* thread_attr, struct sched_param* thread_param) {/*
	thread_param->sched_priority = priority;
	
	pthread_attr_setschedparam (thread_attr, thread_param);
	pthread_attr_setinheritsched (thread_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy (thread_attr, SCHED_RR);*/
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

	//read air humidty
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
	waterTemperature=watertempsensor.adcGetValue(1);
	printf("W temp lida: %d\n", waterTemperature);

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
		//convert
		float Temp_Code = (float)airTemperature;
		result = ((float)175.72 * (float)Temp_Code / (float)256) - (float)46.85;
	
		if((int)result<refTemperature)
			tHeaterPower=100;
		else if((int)result>refTemperature && (int)result<refTemperature+10)
			tHeaterPower=(refTemperature+10-(int)result)*10;
		else
			tHeaterPower=0;
		printf("Temp processada: %f\n", result);
	
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
		float RH_Code = (float)airHumidity;
		result = ((float)125 * (float)RH_Code / (float)256) + (float)6;
	
		if(result<refHumidity)
			tMotorPosition=0;
		else
			tMotorPosition=256;
		printf("Hum processada: %f\n", result);
		
		pthread_mutex_lock(&mutexTargetMotorPosition);
			targetMotorPosition = tMotorPosition;
		pthread_mutex_unlock(&mutexTargetMotorPosition);
		
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
		printf("Lux lida: %d\n", lightLevel);
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
		printf("Set heater to %d\n", tHeaterPower);
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
		printf("Rotate motor to %d degrees\n", tMotorPosition);
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
		printf("Set light to %d\n", tLightPower);
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
		printf("Set water pump to %d\n", wPumpState);
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
	waterPump.init();//GPIO25
	stepMotor.init(13, 6, 5, 26, 0, 5000, 0);//GPIO13,6,5,26
	light.init();//GPIO23
	heater.init();//GPIO24

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
	//setPrio(7, &thread_attr, &thread_param);
	pthread_create(&threadTakePhoto, 0, taskTakePhoto, NULL);
	//setPrio(6, &thread_attr, &thread_param);
	pthread_create(&threadProcessPhoto, 0, taskProcessPhoto, NULL);
	pthread_detach(threadTakePhoto);
	pthread_detach(threadProcessPhoto);



	pthread_t threadSendData, threadSendPhoto;
	//setPrio(4, &thread_attr, &thread_param);
	pthread_create(&threadSendData, 0, taskSendData, NULL);
	//setPrio(5, &thread_attr, &thread_param);
	pthread_create(&threadSendPhoto, 0, taskSendPhoto, NULL);
	pthread_detach(threadSendData);
	pthread_detach(threadSendPhoto);

	pthread_t threadProcessAirTemperature, threadProcessAirHumidity, threadProcessLightLevel;
	//setPrio(3, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirTemperature, 0, taskProcessAirTemperature, NULL);
	//setPrio(3, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirHumidity, 0, taskProcessAirHumidity, NULL);
	//setPrio(3, &thread_attr, &thread_param);	
	pthread_create(&threadProcessLightLevel, 0, taskProcessLightLevel, NULL);
	pthread_detach(threadProcessAirTemperature);
	pthread_detach(threadProcessAirHumidity);
	pthread_detach(threadProcessLightLevel);

	pthread_t threadActuateHeater, threadActuateWindow, threadActuateLight, threadActuateWaterPump;
	//setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateHeater, 0, taskActuateHeater, NULL);
	//setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateWindow, 0, taskActuateWindow, NULL);
	//setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateLight, 0, taskActuateLight, NULL);
	//setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadActuateWaterPump, 0, taskActuateWaterPump, NULL);
	pthread_detach(threadActuateHeater);
	pthread_detach(threadActuateWindow);
	pthread_detach(threadActuateLight);
	pthread_detach(threadActuateWaterPump);

	pthread_t threadCheckWifiDataReception;
	//setPrio(6, &thread_attr, &thread_param);
	pthread_create(&threadCheckWifiDataReception, 0, taskCheckWifiDataReception, NULL);
	pthread_detach(threadCheckWifiDataReception);
	
	pthread_t threadSetAirTemperature, threadSetAirHumidity;
	//setPrio(2, &thread_attr, &thread_param);
	pthread_create(&threadSetAirTemperature, 0, taskSetAirTemperature, NULL);
	//setPrio(2, &thread_attr, &thread_param);
	pthread_create(&threadSetAirHumidity, 0, taskSetAirHumidity, NULL);
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
