#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "airsensor.h"
#include "test.h"

int main(){
	Airsensor as;
	if(as.init(1, 0x40)==0)
		return 0;
	char buf[10];
	buf[0]=0xE3;
	as.cwrite(buf);
	int i=0;
	while(1){
		i++;
		as.cread(buf);
		printf("Leitura %d - %d - %d\n", i, (int)buf[0], as.state());
	}	
	
}
