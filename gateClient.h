#ifndef _GATECLIENT_H_
#define _GATECLIENT_H_

#include "util.h"

#define TOTAL_GATES 5
#define MAX_GATE_QUEUE 2
#define ARRIVE_INTERVAL 1800


typedef struct
{
	int gateId;
	int arrivalTime;
	NodeType *planeList;
} gate;

gate gates[TOTAL_GATES];


void initGate(gate *newGate, int id);
void displayGateInfo(gate*);
int getGateToAssign(airplane* newPlane);
int checkTimeCollision(NodeType *head, airplane *plane);
void addPlaneToGate(int gateId, airplane** plane);
void clearGate(int gateId);


#endif
