#include "main.h"
#include "appCommunication.h"

void tomato(){
	int temp=15, hum=85;
	printf("Temp= %d Hum= %d\n", temp, hum);	
}

void lettuce(){
	int temp=16, hum=60;
	printf("Temp= %d Hum= %d\n", temp, hum);
}

void spinach(){
	int temp=13, hum=92;
	printf("Temp= %d Hum= %d\n", temp, hum);	
}

void cucumber(){
	int temp=27, hum=90;
	printf("Temp= %d Hum= %d\n", temp, hum);
}

void celery(){
	int temp=18, hum=90;
	printf("Temp= %d Hum= %d\n", temp, hum);
}

void pepper(){
	int temp=24, hum=92;
	printf("Temp= %d Hum= %d\n", temp, hum);	
}

void* plant(void* arg){
	std::string command=*(std::string*)arg;
	if(getWord(command,2).compare("Tomato")==0){
		tomato();
	}
	
	if(getWord(command,2).compare("Lettuce")==0){
		lettuce();
	}
	
	if(getWord(command,2).compare("Spinach")==0){
		spinach();
	}
	
	if(getWord(command,2).compare("Cucumber")==0){
		cucumber();
	}
	
	if(getWord(command,2).compare("Celery")==0){
		celery();
	}
	
	if(getWord(command,2).compare("Pepper")==0){
		pepper();
	}
}

void* turn(void* command){
	static bool state=true;
	if(state=!state)
		printf("Desligar\n");
	else
		printf("Ligar\n");	
}

void* dataRequest(void* command){
	printf("Enviar os dados\n");

	int value;
	char msg[128];

	sem_wait(&semaphoreAirTemperature);
	if (!airTemperatureBuffer.check(value)) {/*buffer full*/ }
	sem_post(&semaphoreAirTemperature);

	sprintf(msg, "airTemp %d", value);
	a.sendApp(msg, sizeof(msg));

	sem_wait(&semaphoreAirHumidity);
	if (!airHumidityBuffer.check(value)) {/*buffer full*/ }
	sem_post(&semaphoreAirHumidity);

	sprintf(msg, "airHum %d", value);
	a.sendApp(msg, sizeof(msg));

	sem_wait(&semaphoreWaterTemperature);
	if (!waterTemperatureBuffer.check(value)) {/*buffer full*/ }
	sem_post(&semaphoreWaterTemperature);

	sprintf(msg, "waterTemp %d", value);
	a.sendApp(msg, sizeof(msg));

	sem_wait(&semaphoreLightLevel);
	if (!lightLevelBuffer.check(value)) {/*buffer full*/ }
	sem_post(&semaphoreLightLevel);

	sprintf(msg, "lightLevel %d", value);
	a.sendApp(msg, sizeof(msg));

	if (heater.state())
		strcpy(msg, "heater on");
	else
		strcpy(msg, "heater off");
	a.sendApp(msg, sizeof(msg));

	if (waterPump.state())
		strcpy(msg, "waterPump on");
	else
		strcpy(msg, "waterPump off");
	a.sendApp(msg, sizeof(msg));

	if (light.state())
		strcpy(msg, "light on");
	else
		strcpy(msg, "light off");
	a.sendApp(msg, sizeof(msg));

	if (stepMotor.state())
		strcpy(msg, "window on");
	else
		strcpy(msg, "window off");
	a.sendApp(msg, sizeof(msg));

}
