#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"


int ucmd_parse(Command cmd_list[], const char* delim, const char* in)
{
    if (!in || strlen(in) == 0) return 0;
    if (!cmd_list) return UCMD_CMD_NOT_FOUND;

    delim = (delim) ? delim : UCMD_DEFAULT_DELIMETER;        //if delim is defined -> keep it, if not define as default

    int retval = 0;    				       //return value

    char* s = (char*)malloc(strlen(in) + 1);                 //alocates and copies input string
    
    strcpy(s, in);
    
    int argc = 0;
    char** argv = NULL;

    char* arg = strtok(s, delim);

    while (arg) 
    {
        argc++;
        argv = (char**)realloc(argv, argc * sizeof(*argv));
        argv[argc - 1] = arg;
        arg = strtok(NULL, delim);
    }
		

    if (argc) 
    {
        Command* c = NULL;
        for (Command* p = cmd_list; p->cmd; p++) 
        {
        	if (strcmp(p->cmd, argv[0]) == 0)
        	{
        		c = p;
			break;			//leaves if it has found cmd (avoids unncessary operations)
               }
	       if(p->fn == NULL)	        //failsafe condition to avoid anautorized memory access and cause a crash
	       	break;
        }
        if (c) 
        {
            retval = c->fn(argc, argv);	
        }
        else 
        {
        	retval = UCMD_CMD_NOT_FOUND;
        }
    }
    free(argv);
    free(s);

    return retval;
}

void ucmd_error_handler(int error)
{
	if(error)
	{
		char* error_str;
		switch(error)
		{
			case UCMD_CMD_NOT_FOUND:
				error_str = "Command not found\n";
				break;
			
			case UCMD_CMD_LIST_NOT_FOUND:
				error_str = "Command List not found\n";
				break;
			
			case UCMD_MEM_NOT_VALID:
				error_str = "Memory address not valid, check help for valid addresses\n";
				break;
			
			case UCMD_PORT_NOT_VALID:
				error_str = "GPIO port not valid, check help for valid ports\n";
				break;
			
			case UCMD_ADC_CHANNEL_NOT_VALID:
				error_str = "ADC channel not valid, check help for valid channels\n";
				break;
			
			case UCMD_CMD_LAST_CMD_LOOP:
				error_str = "Command unavailable, use another command\n";
				break;
			
		}
		//print error msg
		printf("%s",error_str);
	}
}
