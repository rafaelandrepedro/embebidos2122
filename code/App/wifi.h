#ifndef WIFI_H
#define WIFI_H

#include <QObject>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>


#define PORTNUM 5002
#define CONNECTION_NUM 2
const char server_ip[] = "192.168.1.98";

void panic(char *msg);
#define panic(m)	{perror(m); abort();}


class Wifi : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(qint16 listen_sd READ listen_sd WRITE setListen_sd)
public:
    explicit Wifi(QObject *parent = nullptr);
   // qint16 listen_sd;

signals:

public slots:
    void init();
    void connectToServer();
    void sendToServer(QString);
    void receiveFromServer();

private:
    bool connected;

};

#endif // WIFI_H

