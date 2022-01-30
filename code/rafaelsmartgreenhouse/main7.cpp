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
#include <mqueue.h>   /* mq_* functions */
#include "database.h"

int main(int count, char* args[])
{
	
	Database db("database.db");
   	db.quarry("CREATE TABLE REPORT("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "AIRTEMPERATURE           FLOAT," \
      "AIRHUMIDITY            FLOAT," \
      "LIGHTLEVEL        FLOAT," \
      "WATERTEMPERATURE         FLOAT);");

	int ID=1;
	int AIRTEMPERATURE=20;
	int AIRHUMIDITY=40;
	int LIGHTLEVEL=50;
	int WATERTEMPERATURE=20;
		
	
	//MAIN LOOP
	while (1)
	{
		sleep(1);		
		char str[1<<8];
		sprintf(str, "INSERT INTO REPORT (ID,AIRTEMPERATURE,AIRHUMIDITY,LIGHTLEVEL,WATERTEMPERATURE) VALUES (%d, %d, %d, %d, %d); ", ID, AIRTEMPERATURE, AIRHUMIDITY, LIGHTLEVEL, WATERTEMPERATURE);
		db.quarry(str);
		ID++;
		AIRTEMPERATURE++;
		AIRHUMIDITY++;
		LIGHTLEVEL--;
		WATERTEMPERATURE--;
	}
}
