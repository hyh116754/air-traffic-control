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
#include <time.h>

#include "msg_struct.h"
#include "util.c"
#include "gateClient.h"
#include "airplaneClient.h"
#include "controlTowerServer.h"

int main() {

	int coid, status;
	gate_client_msg outgoing_msg;
	airplane_msg incoming_msg;
	gate_request_msg request_msg;
	airplane *plane;

	//initialize a list of gates
	for(int i = 0; i< TOTAL_GATES; ++i){
		initGate(&(gates[i]), i);
	}

	//establish a connection to the server's channel
	if ((coid = name_open("controlSystem", 0)) == -1) {
		perror("ConnectAttach");
		return EXIT_FAILURE;
	}

	// maybe using state machine here:
	// the process goes between requesting thread
	// and idling thread just wait for 30 seconds

	int status_one, status_two;

	request_msg.msg_type = REQUEST_MSG_TYPE;

	//receive airplane info from server
	status_one = MsgSend(coid, &request_msg, sizeof(request_msg), &incoming_msg,
			sizeof(incoming_msg));
	if (-1 == status_one) { //was there an error sending to server?
		perror("MsgSend");
		exit(EXIT_FAILURE);
	}

	printf("received incoming airplane info from server.\n");
	plane = incoming_msg.plane;

	//send gate number to server as message
	outgoing_msg.msg_type = GATE_MSG_TYPE;
	int gateNum = getGateToAssign(plane);
	outgoing_msg.gateNo = gateNum;
	int ret;
	printf("Respond with gate number %d\n", gateNum);
	status_two = MsgSend(coid, &outgoing_msg, sizeof(outgoing_msg), &ret,
				sizeof(ret));

	if (-1 == status_two) {
		perror("MsgSend");
		exit(EXIT_FAILURE);
	}

	printf("received flight number: %d\n, received arrival time: %b %d %H:%M", incoming_msg.plane->id, incoming_msg.plane->arrivalTime);

	return EXIT_SUCCESS;
}

void initGate(gate *newGate, int id)
{
  newGate = malloc(sizeof(gate));
  newGate->gateId = id;
  newGate->planeList = NULL;
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
	printf("cannot find a gate to match!");
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
		diff_t = difftime(currNode->data->arrivalTime, plane->arrivalTime);
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
	NodeType *head = gates[gateId].planeList;
	addPlaneToTail(&head, *plane);
	printList(head);
	sortList(&head);
	printList(head);
}


void clearGate(int gateId) {
	deleteList((gates[gateId]).planeList);
	gates[gateId].planeList = NULL;
}
