#include <stdio.h>
#include "wifi.h"

int main()
{
    WifiCOM a;



    char aux[128] = "";
    char sendmsg[128] = "aaaaaa";
    a.init();
    a.connectWifi();
    a.recvApp(aux);
   // a.close();





    printf("%s\n",aux);
    while(1)
        ;
    return 0;
}
