#ifndef APPCOMMUNICATION_H
#define APPCOMMUNICATION_H

#include <stdio.h>
#include <string.h>
#include <sstream>
#include "wifi.h"
#include "parser.h"

void tomato();

void lettuce();

void spinach();

void cucumber();

void celery();

void pepper();

void* plant(void* arg);

void* turn(void* command);

void* dataRequest(void* command);

#endif

