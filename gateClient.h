#ifndef _GATECLIENT_H_
#define _GATECLIENT_H_

#include <time.h>
#include "util.h"

#define TOTAL_GATES 10
#define ARRIVE_INTERVAL 1800


typedef struct
{
	int gateId;
	int flightId;
} gate;


// an array of gate objects
gate *gates;

void printGate(gate* gate);
// this thread will notify every second any client that needs notification
void *update_thread(void * ignore);


#endif
