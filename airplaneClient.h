#ifndef _AIRPLANECLIENT_H_
#define _AIRPLANECLIENT_H_

#include <time.h>
#include "util.h"

#define NUMPLANES      10

airplane airplanes[10];


//functions for flight control
void initPlane(int,int,int,int,int,airplane**);
void printPlane(const airplane*);
void *planeUpdate(void *arg);
void *planeCollisionCheck(void *arg);
void *planeAssignment(void *arg);


#endif
