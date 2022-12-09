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
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#include "msg_struct.h"
#include "util.c"
#include "gateClient.h"
#include "airplaneClient.h"
#include "controlTowerServer.h"


pthread_mutex_t gate_update_mutex;
int coid;

int main() {

	int chid, rcvid, status;
	airplane *plane;

	// connect to server channel
	coid = name_open(ATTACH_POINT,0);
	if (-1 == coid) {
		perror("ConnectAttach");
		exit(EXIT_FAILURE);
	}

	// initialize a list of gates
	for(int i = 0; i< TOTAL_GATES; ++i){
		setGate(&(gates[i]), i, NULL, NULL);
	}

	// initialize the gate update mutex
	pthread_mutex_init(&gate_update_mutex, NULL);

	// send gate info to server
	// create a thread with very high priority here
	if (MsgSend(coid, &msg, sizeof(msg), NULL, 0) == -1)
	{
		perror("MsgSend");
		exit(EXIT_FAILURE);
	}

	// create the client notification thread
	pthread_create(NULL, NULL, update_thread, NULL);

	return EXIT_SUCCESS;
}

void setGate(gate *newGate, int gateId, int flightId, time_t time)
{
  newGate->gateId = gateId;
  newGate->flightId = flightId;
  newGate->arrivalTime = time;
}

void printGates(gate** gateList) {

	for(int i=0; i<TOTAL_GATES; i++){
		printf("gate id: %d\n, flight id: %d\n, arrival time: %s\n",
					gateList[i]->gateId,
					gateList[i]->flightId,
					gateList[i]->arrivalTime);
	}
}

void *update_thread(void * ignore) {
	int errornum;
	my_msg_t *msg;
	while (1) {
		sleep(1);
		pthread_mutex_lock(&gate_update_mutex);
		printf("request gate update from server\n");
		msg->type = GATE_UPDATE_MSG;
		gate received_gates[TOTAL_GATES];
		MsgSend(coid, &msg, sizeof(msg), &received_gates, sizeof(received_gates));
		printf("server returns a list of gates. update gates now.\n");
		memcpy(gates, received_gates, sizeof(gates));

		pthread_mutex_unlock(&gate_update_mutex);

	}
	return NULL;
}
