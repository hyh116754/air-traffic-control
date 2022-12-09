#ifndef _GATECLIENT_H_
#define _GATECLIENT_H_

#include <time.h>
#include "util.h"

#define TOTAL_GATES 5
#define MAX_GATE_QUEUE 2
#define ARRIVE_INTERVAL 1800


typedef struct
{
	int gateId;
	int flightId;
	time_t arrivalTime;
} gate;


// an array of gate objects
gate gates[TOTAL_GATES];


void setGate(gate *newGate, int gateId, int flightId, time_t time);
void printGates(gate** gateList);
// this thread will notify every second any client that needs notification
void * update_thread(void * ignore);


#endif
