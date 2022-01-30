#ifndef WIFI_H
#define WIFI_H

#include <QObject>
#include <stdio.h>
#include <winsock2.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

#define PORTNUM 5000
#define CONNECTION_NUM 2

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

private:
    qint16 listen_sd;
    struct sockaddr_in addr;

};

#endif // WIFI_H
