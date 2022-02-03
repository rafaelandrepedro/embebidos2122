#ifndef MAIN
#define MAIN

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

#include "database.h"
#include "airsensor.h"
#include "LDRsensor.h"
#include "WaterTempsensor.h"


#include "buffer.h"

#include "parser.h"

#include "appCommunication.h"

#include "heater.h"
#include "stepmotor.h"
#include "light.h"
#include "waterpump.h"



#ifndef NULL
#define NULL 0
#endif


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

/*message queue*/
mqd_t msgq_id;

/*database*/
Database db("database.db");

/*wifi*/
WifiCOM a;

#endif
