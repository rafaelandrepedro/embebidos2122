#compile
make compileserver
make compileclient

#execute (in separated terminals)
#in the server terminal
make runserver
#in each client terminal
make runclient

#the clients are initialized in "Send mode only"

#to change the client mode write in the client terminal "/mode_*" where * is S (send), R (recieve) or B (both)

#in the client terminal send the first message and the other clients shall recieve