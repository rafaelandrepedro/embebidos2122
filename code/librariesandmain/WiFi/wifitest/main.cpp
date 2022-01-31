#include <stdio.h>
#include "wifi.h"


int main()
{
    WifiCOM a;


    char msg[128] = "";
    a.init();
    if(a.connectWifi())
       printf("CONNECTED!!\n");
    else
       printf("ERROR\n");

    //a.recvApp(msg, sizeof(msg));
   // a.close();
    a.recvApp(msg, sizeof(msg));





    printf("%s\n",msg);
    //while(1)
      //  ;
    return 0;
}
