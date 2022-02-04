#include <stdio.h>
#include <string.h>
#include <sstream>
#include "wifi.h"
#include "parser.h"















WifiCOM a;

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
	static bool st=true;
	if(st=!st){
	strcpy(msg, "20");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "30");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "40");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "50");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "On");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "Off");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "On");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "Off");
	a.sendApp(msg, sizeof(msg));
	}
	else{
	strcpy(msg, "30");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "20");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "50");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "40");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "Off");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "On");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "Off");
	a.sendApp(msg, sizeof(msg));
	strcpy(msg, "On");
	a.sendApp(msg, sizeof(msg));
	}
}

int main()
{
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
    		printf("%s\n", msg);
    		parser.search(std::string(msg));
    	}
    a.sendApp(msg, sizeof(msg));
    return 0;
}
