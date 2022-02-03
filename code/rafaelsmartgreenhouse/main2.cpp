#include "main2.h"

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
/**
 * @brief takes a photo and stores in the Raspberry Pi
 *
 * @return void
 */
void* taskTakePhoto(void*) {
	while (1) {
		system("raspistill -o [nome]");
		sleep(86400);
	}
	return NULL;
}

//?????????????????????????????????????????????????????????????????????????????????????????????
/**
 * @brief stores the sensor values into the database
 *
 * @return void
 */
void* taskSendData(void*) {
	static int ID=0;
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
	
	char str[1<<8];
	sprintf(str, "INSERT INTO REPORT (ID,AIRTEMPERATURE,AIRHUMIDITY,LIGHTLEVEL,WATERTEMPERATURE) VALUES (%d, %d, %d, %d, %d); ", ID, airTemperature, airHumidity, lightLevel, waterTemperature);
	db.quarry(str);
	ID++;
	return NULL;
}

//?????????????????????????????????????????????????????????????????????????????????????????????

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
	
		if((int)result<=refTemperature-10)
			tHeaterPower=100;
		else if((int)result>refTemperature-10 && (int)result<refTemperature)
			tHeaterPower=(refTemperature-(int)result)*10;
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
	
		if((int)result<=refHumidity-10)
			tMotorPosition=256;
		else if((int)result>refHumidity-10 && (int)result<refHumidity)
			tMotorPosition=(refHumidity-(int)result)*25.6;
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


		if(lightLevel<40){
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

//?????????????????????????????????????????????????????????????????????????????????????????????

/**
 * @brief checks the heater actuation variable and sets the heater to the target temperature
 *
 * @return void
 */
void* taskActuateHeater(void*) {//GPIO24
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
void* taskActuateLight(void*) {//GPIO23
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
void* taskActuateWaterPump(void*) {//GPIO25
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

//?????????????????????????????????????????????????????????????????????????????????????????????

/**
 * @brief searches for a client and connects to it if found
 *
 * @return void
 */
void* taskCheckWifiDataReception(void*) {
	Parser parser;
    
	parser.add(&plant,"plant");
	parser.add(&turn,"turn");
	parser.add(&dataRequest,"dataRequest");

	char msg[128] = "";
	a.init();
	if(a.connectWifi())
		printf("CONNECTED!!\n");
	else
		printf("ERROR\n");

	while(1){
    		a.recvApp(msg, sizeof(msg));
    		printf("App > %s\n", msg);
    		parser.search(std::string(msg));
    	}
	return NULL;
}
/**
 * @brief sets the air temperature reference to a value given from the mobile app
 *
 * @return void
 */
void taskSetAirTemperature(int) {
	return NULL;
}
/**
 * @brief sets the air humidity reference to a value given from the mobile app
 *
 * @return void
 */
void taskSetAirHumidity(int) {
	return NULL;
}

//?????????????????????????????????????????????????????????????????????????????????????????????

void acquireSensorData(int){
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

//?????????????????????????????????????????????????????????????????????????????????????????????
int main(int count, char* args[])
{

	//MAIN SETUP
		
	//set text color to normal
	printf("\033[0;37m");

	a.init();
	if(a.connectWifi())
		printf("CONNECTED!!\n");
	else
		printf("ERROR\n");
	
	
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
	
   	db.quarry("CREATE TABLE REPORT("  \
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
	pthread_t threadTakePhoto;
	setPrio(1, &thread_attr, &thread_param);
	pthread_create(&threadTakePhoto, 0, taskTakePhoto, NULL);
	pthread_detach(threadTakePhoto);

	//threads: send to database
	pthread_t threadSendData;
	setPrio(4, &thread_attr, &thread_param);
	pthread_create(&threadSendData, 0, taskSendData, NULL);
	pthread_detach(threadSendData);
	
	//threads: process raw values and send actuation controls
	pthread_t threadProcessAirTemperature, threadProcessAirHumidity, threadProcessLightLevel;
	setPrio(5, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirTemperature, 0, taskProcessAirTemperature, NULL);
	setPrio(5, &thread_attr, &thread_param);
	pthread_create(&threadProcessAirHumidity, 0, taskProcessAirHumidity, NULL);
	setPrio(5, &thread_attr, &thread_param);	
	pthread_create(&threadProcessLightLevel, 0, taskProcessLightLevel, NULL);
	pthread_detach(threadProcessAirTemperature);
	pthread_detach(threadProcessAirHumidity);
	pthread_detach(threadProcessLightLevel);
	
	//threads: control the actuators
	pthread_t threadActuateHeater, threadActuateWindow, threadActuateLight, threadActuateWaterPump;
	setPrio(7, &thread_attr, &thread_param);
	pthread_create(&threadActuateHeater, 0, taskActuateHeater, NULL);
	setPrio(7, &thread_attr, &thread_param);
	pthread_create(&threadActuateWindow, 0, taskActuateWindow, NULL);
	setPrio(7, &thread_attr, &thread_param);
	pthread_create(&threadActuateLight, 0, taskActuateLight, NULL);
	setPrio(7, &thread_attr, &thread_param);
	pthread_create(&threadActuateWaterPump, 0, taskActuateWaterPump, NULL);
	pthread_detach(threadActuateHeater);
	pthread_detach(threadActuateWindow);
	pthread_detach(threadActuateLight);
	pthread_detach(threadActuateWaterPump);
	
	//threads: connect to mobile app
	pthread_t threadCheckWifiDataReception;
	setPrio(2, &thread_attr, &thread_param);
	pthread_create(&threadCheckWifiDataReception, 0, taskCheckWifiDataReception, NULL);
	pthread_detach(threadCheckWifiDataReception);
	
	
		
	
	msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG, NULL);
	if (msgq_id == (mqd_t)-1) {
		perror("In mq_open()");
		exit(1);
	}
	
	mq_close(msgq_id);
	
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
		signal(SIGALRM, acquireSensorData); // set signal (alarm)
		struct itimerval itv1;
		itv1.it_interval.tv_sec = itv1.it_value.tv_sec = 5;
		itv1.it_interval.tv_usec = itv1.it_value.tv_usec = 0;
		setitimer(ITIMER_REAL, &itv1, NULL);
			
		while(1){	
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
		std::string msg = std::to_string(values[0]) + std::to_string(values[1]) + std::to_string(values[2]) + std::to_string(values[3]);
		
		// sending the message      --  mq_send() 
		mq_send(msgq_id, std::to_string(values[0]).c_str(), strlen(std::to_string(values[0]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[1]).c_str(), strlen(std::to_string(values[1]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[2]).c_str(), strlen(std::to_string(values[2]).c_str())+1, msgprio);
		mq_send(msgq_id, std::to_string(values[3]).c_str(), strlen(std::to_string(values[3]).c_str())+1, msgprio);

		
	}
}
