#********SETUP********
#compile
make compileserver
make compileclient

#execute (in separated terminals)
#in the server terminal
make runserver
#in each client terminal
make runclient

#********INFO********
#the clients are initialized in "Send and recieve mode"

#to change the client mode write in the client terminal "/mode_*" where * is S (send), R (recieve) or B (both)

#line 109 of the server can be uncommented to check the periodic afk checking

#********TESTING********
#TEST BROADCAST
#in the client1 terminal send the first message and the other clients shall recieve the message broadcasted

#TEST MODES
#in the client1 set the mode to /mode_S
#if client2 sends a message, client1 won't receive it because client1 is set to only send (server will print the message)
#
#in the client2 set the mode to /mode_R
#in the client1 turn the mode back to /mode_B
#if client2 sends a message, client1 won't receive it because client2 is set to only receive (server will not print the message)
