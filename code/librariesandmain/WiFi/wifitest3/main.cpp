#include <stdio.h>
#include <string.h>
#include <sstream>
#include "wifi.h"


int main()
{
    //Parser parser;
    Wifi a;
    //parser.add(&plant,"plant");
    //parser.add(&turn,"turn");
    //parser.add(&dataRequest,"dataRequest");

    char msg[128] = "";
    a.init();
    if(a.connectToServer())
       printf("CONNECTED!!\n");
    else
       printf("ERROR\n");

     a.receive_image();
    //a.sendApp(msg, sizeof(msg));
    return 0;
}
