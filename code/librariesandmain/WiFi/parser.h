#ifndef __UCMD_H
#define __UCMD_H

#include "limits.h"
#include "stdint.h"

/* ERROR DEFINES */
#define UCMD_CMD_NOT_FOUND 		INT_MIN			

#define UCMD_INPUT_STRING_INVALID 	INT_MIN+1

#define UCMD_CMD_LIST_NOT_FOUND 	INT_MIN+2

#define UCMD_MEM_NOT_VALID		INT_MIN+3

#define UCMD_PORT_NOT_VALID		INT_MIN+4

#define UCMD_ADC_CHANNEL_NOT_VALID 	INT_MIN+5

#define UCMD_CMD_LAST_CMD_LOOP	INT_MIN+6

#define UCMD_DEFAULT_DELIMETER " "

typedef int (*Command_cb)(int, char* []);

typedef struct Command
{
	const char *cmd;  //name of command  
	const char *help; //help text of command
	Command_cb fn;    //Command callback
}Command;

#endif
