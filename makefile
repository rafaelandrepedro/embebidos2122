CC = gcc
#CC = ~/buildroot/buildroot-2021.05.2/output/host/bin/./aarch64-buildroot-linux-gnu-gcc

ADDR = 0.0.0.0  
PORT = 4500


compileserver: 
	gcc tcpserver.c -o tcpserver.elf -lpthread
		
runserver:	
	./tcpserver.elf $(PORT)

compileclient: 
	$(CC) tcpclient.c -o tcpclient.elf -lpthread -lrt
	
runclient:
	./tcpclient.elf $(ADDR) $(PORT)
	
clr: 
	rm *.elf *.o
