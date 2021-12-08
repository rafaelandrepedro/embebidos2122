CC = gcc
#CC = buildroot/buildroot-2021.02.5/output/host/bin/arm-buildroot-linux-gnueabihf-gcc

ADDR = 0.0.0.0  
PORT = 4200


compileserver: 
	gcc tcpserver.c -g -o tcpserver.elf -lpthread
		
runserver:	
	./tcpserver.elf $(PORT)

compileclient: 
	$(CC) tcpclient.c -g -o tcpclient.elf -lpthread -lrt
	
runclient:
	./tcpclient.elf $(ADDR) $(PORT)
	
clr: 
	rm *.elf *.o
	
clrall: 
	rm *.elf *.o *.c
