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

#include "airSensor.h"
#include "waterSensor.h"
#include "lightSensor.h"

#include "heater.h"
#include "stepMotor.h"
#include "LED.h"
#include "waterPump.h"

#define BUFFER_SIZE 8

template <typename BufferType>
class Buffer {
private:
	BufferType* values[BUFFER_SIZE];
	int addPtr, removePtr;
public:
	Buffer(void);
	~Buffer(void);
	bool add(BufferType value);
	bool remove(BufferType* value);
};

template <typename BufferType>
Buffer<BufferType>::Buffer(void) {
	addPtr = 0;
	removePtr = 0;
}

template <typename BufferType>
Buffer<BufferType>::~Buffer(void) {}

template <typename BufferType>
Buffer<BufferType>::add(BufferType value) {
	if ((addPtr + 1 == removePtr) || (addPtr == BUFFER_SIZE && removePtr == 0))
		return 0;
	values[addPtr] = value;
	addPtr++;
	if (addPtr == BUFFER_SIZE)
		addPtr = 0;
	return 1;
}

template <typename BufferType>
Buffer<BufferType>::remove(BufferType* value) {
	if (addPtr == removePtr)
		return 0;
	*value = values[removePtr];
	removePtr++;
	if (removePtr == BUFFER_SIZE)
		removePtr = 0;
	return 1;
}


int main()
{
	cout << myMax<int>(3, 7) << endl;  // Call myMax for int
	cout << myMax<double>(3.0, 7.0) << endl; // call myMax for double
	cout << myMax<char>('g', 'e') << endl;   // call myMax for char

	return 0;
}

Buffer<float> airTemperatureBuffer;
Buffer<float> airHumidityBuffer;
Buffer<float> waterTemperatureBuffer;
Buffer<float> lightLevelBuffer;
//photoBuffer
//processedPhotoBuffer

float targetMotorPosition;
float targetHeaterPower;
float targetLightPower;
float waterPumpState;


void panic(char* msg);
#define panic(m)		{perror(m); abort();}


//슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬슬�

void* taskReadSensors(void);

void* taskTakePhoto(void);
void* taskProcessPhoto(void);

void* taskSendData(void);
void* taskSendPhoto(void);

void* taskProcessAirTemperature(void);
void* taskProcessAirHumidity(void);
void* taskProcessLightLevel(void);

void* taskActuateHeater(void);
void* taskActuateWindow(void);
void* taskActuateLight(void);
void* taskActuateWaterPump(void);

void* taskCheckWifiDataReception(void);
void* taskSetAirTemperature(void);
void* taskSetAirHumidity(void);


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

	pthread_t threadCheckWifiDataReception, threadSetAirTemperature, threadSetAirHumidity;
	pthread_create(&threadCheckWifiDataReception, 0, taskCheckWifiDataReception, VOID);
	pthread_create(&threadSetAirTemperature, 0, taskSetAirTemperature, VOID);
	pthread_create(&threadSetAirHumidity, 0, taskSetAirHumidity, VOID);
	pthread_detach(threadCheckWifiDataReception);
	pthread_detach(threadSetAirTemperature);
	pthread_detach(threadSetAirHumidity);

	//###create thread template###
		//int arguments1 = 0;
		//pthread_t thread1;
		//pthread_create(&thread1, 0, task1, &arguments1);       /* start thread */
		//pthread_detach(thread1);                    /* don't track it */

	//###create periodic task template###
		//signal(SIGALRM, periodicTask1); // set signal (alarm)
		//struct itimerval itv1;
		//itv1.it_interval.tv_sec = itv1.it_value.tv_sec = 5;
		//itv1.it_interval.tv_usec = itv1.it_value.tv_usec = 0;
		//setitimer(ITIMER_REAL, &itv1, NULL); /* send signal to process for every 5 seconds*/

	//###create daemon template###
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
