/**
 * gateClient.c
 *
 * Manage gate info.
 * Act as a client to receive and response server requests.
 *
 * This client will send message of the gate number to assign to server.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <process.h>
#include <sys/dispatch.h>

#include "gateClient.h"
#include "controlTowerServer.h"

int main() {

	int coid, incoming_msg, status;
	assign_gate_msg msg;
	flight_arrival_msg incoming_msg;
	gate gates[TOTAL_GATES];

	//initialize a list of gates
	for(int i = 0; i< TOTAL_GATES; ++i){
		initGate(&gate[i], i);
		displayGateInfo(gate[i]);
	}

	//establish a connection to the server's channel
	if ((coid = name_open("controlSystem", 0)) == -1) {
			return EXIT_FAILURE;
	}

	//send gate number to server as message
	msg.msg_type = GATE_MSG_TYPE;
	msg.gateNo = getGateToAssign();
	printf("Respond with gate number %d\n", 5);


	// incoming msg shall include flight number and time

	status = MsgSend(coid, &msg, sizeof(msg), &incoming_msg,
				sizeof(incoming_msg));

	if (-1 == status) {
		perror("MsgSend");
		exit(EXIT_FAILURE);
	}

	//receive flight number, display the flight number

	printf("received flight number: %d\n, received arrival time: ", incoming_msg);

	return EXIT_SUCCESS;
}

void initGate(gate ** newGate, int id)
{
  *newGate = malloc(sizeof(gate));
  (*newGate)->id = id;
  (*newGate)->planeList = NULL;
}

void displayGateInfo(gate* gate) {

	printf("gate id: %d\n", gate->gateId);
	printList(gate->planeList);
}

int getGateToAssign(airplane* newPlane) {

	// for each gate check if either empty or have no time collision
	for(int i=0; i<TOTAL_GATES; i++)
	{
		int res = checkTimeCollision(gates[i].planeList, newPlane);
		if (res > 0) return i;
	}
	print("cannot find a gate to match!");
	return -1;


}

int checkTimeCollision(NodeType *head, airplane *plane)
{
	NodeType *currNode = head;
	double diff_t;
	int flag = 0;

	if (head == NULL) return TRUE;

	while (currNode)
	{
		// if timediff < 30 mins, return false
		diff_t = difftime(currNode->data->arrivalTime, plane->arrivalTime)
		if(fabs(diff_t) < ARRIVE_INTERVAL)
		{
			return FALSE;
		}
		currNode = currNode->next;
	}

	return TRUE;
}

void addPlaneToGate(int gateId, airplane** plane) {
	// decide queue on the arrival time
	NodeType *head = gate[gateId].planeList;
	addPlaneToTail(&head, *plane);
	printList(head);
	sortList(&head);
	printList(head);
}


void clearGate(int gateId) {
	deleteList(gates[gateId].planeList);
	gates[gateId].planeList = NULL;
}
