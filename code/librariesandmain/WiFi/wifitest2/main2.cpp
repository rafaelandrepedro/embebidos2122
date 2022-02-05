#include <stdio.h>
#include <string.h>
#include <sstream>
#include "wifi2.h"



int main()
{
    //Parser parser;
    WifiCOM a;
    //parser.add(&plant,"plant");
    //parser.add(&turn,"turn");
    //parser.add(&dataRequest,"dataRequest");

    char msg[128] = "";
    a.init();
    if(a.connectWifi())
       printf("CONNECTED!!\n");
    else
       printf("ERROR\n");
	
	a.send_image();
	
    //a.sendApp(msg, sizeof(msg));
    return 0;
}
