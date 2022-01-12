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
#include <semaphore.h>
//#include <sys/semaphore.h>

//#include "airSensor.h"
//#include "waterSensor.h"
//#include "lightSensor.h"

#include "heater.h"
#include "stepmotor.h"
#include "light.h"
#include "waterpump.h"

#include "buffer.h"

#define BUFFER_SIZE 8

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
float targetMotorPosition;
float targetHeaterPower;
float targetLightPower;
float waterPumpState;

/*Critical error routine*/
void panic(char* msg);
#define panic(m)		{perror(m); abort();}

/*classes*/
WaterPump waterPump;
StepMotor stepMotor;
Light light;
Heater heater;


//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskReadSensors(void) {
	setPrio(unsigned int priority)

	float airTemperature, airHumidity;

	//read air temperature

	//read air humidty

	sem_wait(&semaphoreAirTemperature);
		if (!airTemperatureBuffer.add(airTemperature)) {/*buffer full*/ }
	sem_post(&semaphoreAirTemperature);

	sem_wait(&semaphoreAirHumidity);
		if (!airHumidityBuffer.add(airHumidity)) {/*buffer full*/ }
	sem_post(&semaphoreAirHumidity);

	float waterTemperature;

	//read water temperature

	sem_wait(&semaphoreWaterTemperature);
		if (!waterTemperatureBuffer.add(waterTemperature)) {/*buffer full*/ }
	sem_post(&semaphoreWaterTemperature);

	float lightLevel;

	//read light level

	sem_wait(&semaphoreLightLevel);
		if (!lightLevelBuffer.add(lightLevel)) {/*buffer full*/ }
	sem_post(&semaphoreLightLevel);

}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskTakePhoto(void) {
	setPrio(1);
}
void* taskProcessPhoto(void) {
	setPrio(2);
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskSendData(void) {
	setPrio(4);

	float airTemperature, airHumidity;

	sem_wait(&semaphoreAirTemperature);
		while (!airTemperatureBuffer.remove(&airTemperature)) {/*buffer empty*/ }
	sem_post(&semaphoreAirTemperature);

	sem_wait(&semaphoreAirHumidity);
		while (!airHumidityBuffer.remove(&airHumidity)) {/*buffer empty*/ }
	sem_post(&semaphoreAirHumidity);

	float waterTemperature;

	sem_wait(&semaphoreWaterTemperature);
		while (!waterTemperatureBuffer.remove(&waterTemperature)) {/*buffer empty*/ }
	sem_post(&semaphoreWaterTemperature);

	float lightLevel;

	sem_wait(&semaphoreLightLevel);
		while (!lightLevelBuffer.remove(&lightLevel)) {/*buffer empty*/ }
	sem_post(&semaphoreLightLevel);

	//insert in the database

}

void* taskSendPhoto(void) {
	setPrio(3);
	//photo struct

	sem_wait(&semaphoreProcessedPhotoBuffer);
		while (!lightLevelBuffer.remove(/*//photo struct*/)) {/*buffer empty*/ }
	sem_post(&semaphoreProcessedPhotoBuffer);

	//insert in the database
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskProcessAirTemperature(void) {
	setPrio(5);
	
	float airTemperature;
	float result;

	sem_wait(&semaphoreAirTemperature);
		while (!airTemperatureBuffer.remove(&airTemperature)) {/*buffer empty*/ }
	sem_post(&semaphoreAirTemperature);

	//convert
	float Temp_Code = airTemperature;
	result = (175.72 * Temp_Code / 65536) + 46.85;


	pthread_mutex_lock(&mutexTargetHeaterPower);
		targetHeaterPower = result;
	pthread_mutex_unlock(&mutexTargetHeaterPower);

}

void* taskProcessAirHumidity(void) {
	setPrio(5);

	float airHumidity;
	float result;

	sem_wait(&semaphoreAirHumidity);
		while (!airTemperatureBuffer.remove(&airHumidity)) {/*buffer empty*/ }
	sem_post(&semaphoreAirHumidity);

	//convert
	float RH_Code = airHumidity;
	result = (125 * RH_Code / 65536) + 6;

	pthread_mutex_lock(&mutexTargetMotorPosition);
		targetMotorPosition = result;
	pthread_mutex_unlock(&mutexTargetMotorPosition);

}

void* taskProcessLightLevel(void) {
	setPrio(5);

	float airTemperature;
	float result;

	sem_wait(&semaphoreLightLevel);
		while (!airTemperatureBuffer.remove(&lightLevel)) {/*buffer empty*/ }
	sem_post(&semaphoreLightLevel);

	//convert
	float R10 = 15000;	//resistance at 10 lux
	float R100 = 2500;	//resistance at 100 lux
	result = 2.7958-log(lightLevel)*1.3979;

	pthread_mutex_lock(&mutexTargetLightPower);
		targetLightPower = result;
	pthread_mutex_unlock(&mutexTargetLightPower);

}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskActuateHeater(void) {
	setPrio(7);
	pthread_mutex_lock(&mutexTargetHeaterPower);
	targetHeaterPower;
	pthread_mutex_unlock(&mutexTargetHeaterPower);
	//

}

void* taskActuateWindow(void) {
	setPrio(7);
	pthread_mutex_lock(&mutexTargetMotorPosition);
	targetMotorPosition;
	pthread_mutex_unlock(&mutexTargetMotorPosition);
	//

}

void* taskActuateLight(void) {
	setPrio(7);
	pthread_mutex_lock(&mutexTargetLightPower);
	targetLightPower;
	pthread_mutex_unlock(&mutexTargetLightPower);
	//

}

void* taskActuateWaterPump(void) {
	setPrio(7);
	pthread_mutex_lock(&mutexWaterPumpState);
	waterPumpState;
	pthread_mutex_unlock(&mutexWaterPumpState);
	//

}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½

void* taskCheckWifiDataReception(void) {}
void* taskSetAirTemperature(void) {
	setPrio(6);
}
void* taskSetAirHumidity(void) {
	setPrio(6);
}

//½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½½
int main(int count, char* args[])
{
	//MAIN SETUP
	waterPump.init();
	stepMotor.init();
	light.init();
	heater.init();

	//declare semaphores
	if (sem_init(&airTemperatureBuffer, 0, 1); != 0)
	{
		// Error: initialization failed
	}
	

	pthread_t threadTakePhoto, threadProcessPhoto;
	pthread_create(&threadTakePhoto, 0, taskTakePhoto, VOID);
	pthread_create(&threadProcessPhoto, 0, taskProcessPhoto, VOID);
	pthread_detach(threadTakePhoto);
	pthread_detach(threadProcessPhoto);

	pthread_t threadSendData, threadSendPhoto;
	pthread_create(&threadSendData, 0, taskSendData, VOID);
	pthread_create(&threadSendPhoto, 0, taskSendPhoto, VOID);
	pthread_detach(threadSendData);
	pthread_detach(threadSendPhoto);

	pthread_t threadProcessAirTemperature, threadProcessAirHumidity, threadProcessLightLevel;
	pthread_create(&threadProcessAirTemperature, 0, taskProcessAirTemperature, VOID);
	pthread_create(&threadProcessAirHumidity, 0, taskProcessAirHumidity, VOID);
	pthread_create(&threadProcessLightLevel, 0, taskProcessLightLevel, VOID);
	pthread_detach(threadProcessAirTemperature);
	pthread_detach(threadProcessAirHumidity);
	pthread_detach(threadProcessLightLevel);

	pthread_t threadActuateHeater, threadActuateWindow, threadActuateLight, threadActuateWaterPump;
	pthread_create(&threadActuateHeater, 0, taskActuateHeater, VOID);
	pthread_create(&threadActuateWindow, 0, taskActuateWindow, VOID);
	pthread_create(&threadActuateLight, 0, taskActuateLight, VOID);
	pthread_create(&threadActuateWaterPump, 0, taskActuateWaterPump, VOID);
	pthread_detach(threadActuateHeater);
	pthread_detach(threadActuateWindow);
	pthread_detach(threadActuateLight);
	pthread_detach(threadActuateWaterPump);

	pthread_t threadCheckWifiDataReception;
	pthread_create(&threadCheckWifiDataReception, 0, taskCheckWifiDataReception, VOID);
	pthread_detach(threadCheckWifiDataReception);

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

	//MAIN LOOP
		while (1)
		{
			taskReadSensors();
		}
}
