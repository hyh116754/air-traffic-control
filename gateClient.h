#ifndef _GATECLIENT_H_
#define _GATECLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>

#define TOTAL_GATES 5
#define MAX_GATE_QUEUE 2
#define GATE_MSG_TYPE (_IO_MAX + 1)
#define ARRIVE_INTERVAl 1800;


typedef struct
{
	int gateId;
	int arrivalTime;
	NodeType *planeList;
} gate;


void initGate(gate ** newGate, int id);
void displayGateIndo(gate* gate);
int getGateToAssign(airplane* newPlane);
int checkTimeCollision(NodeType *head, airplane *plane);
void addPlaneToGate(int gateId, airplane** plane);
void clearGate(int gateId);


#endif
