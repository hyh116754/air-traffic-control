#ifndef _GATEMANAGER_H_
#include "header.h"

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

// just send 2 int, no pluse
typedef struct
{
	unit16_t msg_type;
	int gateNo;
} assign_gate_msg;


void initGate(gate ** newGate, int id);
void displayGateIndo(gate* gate);
int getGateToAssign(airplane* newPlane);
int checkTimeCollision(NodeType *head, airplane *plane);
void addPlaneToGate(int gateId, airplane** plane);
void clearGate(int gateId);


#endif
