/**
 * controlTowerServer.c
 *
 * Server in the control tower for communication.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/iomsg.h>

#include "controlTowerServer.h"
#include "airplaneClient.h"
#include "gateClient.h"
#include "util.h"
#include "msg_struct.h"

int main(void) {

	name_attach_t *attach;
	my_msg_t msg;
	int rcvid, status, total_gates;

	// server maintain a list of gates
	gate *gateList;

	//creating a channel
	if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
	   return EXIT_FAILURE;
	}
	printf("Control Tower Server Channel created.\n");

	while(1)
	{
		printf("Control Tower is Waiting for a message...\n");
		//code to receive msg or pulse from client
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

		//error checking
		if(rcvid == -1) {
			perror("MsgReceive");
			exit(EXIT_FAILURE);
		}
		// we are not expecting a pulse
		if(rcvid == 0){
		   switch (msg.pulse.code) {
			   case _PULSE_CODE_DISCONNECT:
					printf("disconnect from a client\n");
					break;
			   default:
				   printf("Cannot resolve pulse -> code: %d ... val: %d\n",msg.pulse.code,msg.pulse.value.sival_int);
		   }
		}
	  //receive a message
	   else if (rcvid > 0){
		   //print received message
		   switch (msg.type) {
			case _IO_CONNECT:
				printf("got an _IO_CONNECT.\n");
				if (-1 == MsgReply(rcvid, EOK, NULL, 0)) {
					perror("MsgReply");
					break;
				}
				break;

			case GATE_LIST_MSG:
				// receive a list of gates from gate client.
				gateList = malloc(total_gates * sizeof (gate));
				memcpy(gateList, src, sizeof(gateList));
				status = MsgReply(rcvid, EOK, NULL, 0);
				if (-1 == status) {
					perror("MsgReply");
					exit(EXIT_FAILURE);
				}
				printf("memcpy finished.\n");
				break;
			case GATE_UPDATE_MSG:
				// receive gate update request, will send gatelist
				printf("received gate update request. sending update.\n");
				status = MsgReply(rcvid, EOK, &gateList, sizeof(gateList));
				if (-1 == status) {
					perror("MsgReply");
					exit(EXIT_FAILURE);
				}
				break;
			case AIRPLANE_MSG_TYPE:
				// receives airplane info, will reply with a gate number
				printf("Received a airplane message.\n");
				int res = getGateToAssign(gateList, msg.plane, total_gates);
				status = MsgReply(rcvid, EOK, &res, sizeof(res));
				if (-1 == status) {
					perror("MsgReply");
					exit(EXIT_FAILURE);
				}
				break;
			   break;
		   default:
				/* some other unexpected message */
				printf("unexpected message type: %d\n", msg.type);
				if (-1 == MsgError(rcvid, ENOSYS)) {
					perror("MsgError");
				}
				break;
		   }
	   }
	}
   //remove the name from the namespace and destroy the channel
   name_detach(attach,0);
   return 0;
}

int getGateToAssign(gate* gateList, airplane* newPlane, int total_gates) {
	// for each gate check if it's empty
	for(int i=0; i<total_gates; i++)
	{
		if(gateList[i].flightId == NULL) {
			gateList[i].flightId = newPlane->id;
			gateList[i].arrivalTime = newPlane->arrivalTime;
			return i;
		}
	}
	printf("cannot find a gate to match!");
	return -1;
}

