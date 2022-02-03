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
	char msg[128];
	strcpy(msg, "airTemp 20");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "airHum 30");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "waterTemp 40");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "lightLevel 50");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "heater on");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "waterPump off");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "light on");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "window off");
	a.sendApp(msg, sizeof(msg));
}
