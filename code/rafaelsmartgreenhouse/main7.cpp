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
#include <stdio.h>
#include <stdlib.h>

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
	float AIRTEMPERATURE=22.4;
	float AIRHUMIDITY=79.24;
	float LIGHTLEVEL=46;
	float WATERTEMPERATURE=37;
		
	
	//MAIN LOOP
	while (1)
	{
		sleep(1);		
		char str[1<<8];
		sprintf(str, "INSERT INTO REPORT (ID,AIRTEMPERATURE,AIRHUMIDITY,LIGHTLEVEL,WATERTEMPERATURE) VALUES (%d, %d, %d, %d, %d); ", (int)ID, (int)AIRTEMPERATURE, (int)AIRHUMIDITY, (int)LIGHTLEVEL, (int)WATERTEMPERATURE);
		db.quarry(str);
		ID+=1;
		AIRTEMPERATURE+=(rand()%10-5)*0.2;
		AIRHUMIDITY+=(rand()%10-5)*0.2;
		LIGHTLEVEL+=(rand()%10-5)*0.2;
		WATERTEMPERATURE+=(rand()%10-5)*0.2;
	}
}
