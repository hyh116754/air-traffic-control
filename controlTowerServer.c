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
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "controlTowerServer.h"
#include "airplaneClient.h"
#include "gateClient.h"
#include "util.h"
#include "msg_struct.h"

shmem_t *ptr = NULL;

int main(void) {

	name_attach_t *attach;
	my_msg_t msg;
	int rcvid, status, fd;


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

		if(rcvid == 0){
		   switch (msg.pulse.code) {
		   case SHMEM_READY_PULSE:
			   // receive a pulse for shmem ready
				fd = shm_open(SHMEM_NAME, O_RDWR, 0);
				if (fd == -1)
				{
					perror("shm_open");
					exit(EXIT_FAILURE);
				}

				// open a ptr for shmem
				ptr = mmap(0, sizeof(shmem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
				if (ptr == MAP_FAILED)
				{
					perror("mmap");
					exit(EXIT_FAILURE);
				}
				close(fd);

				sem_wait(&ptr->semaphore);
				printf("total gates: %d\n", ptr->total_gates);

				for(int i = 0; i< ptr->total_gates; ++i){
					printf("gate id: %d, airplane id: %d.\n", ptr->gates[i].gateId, ptr->gates[i].flightId);
				}

				sem_post(&ptr->semaphore);

			   break;
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
			case AIRPLANE_GATE_REQUEST:
				// receive airplane request for gate assignment
				printf("received airplane gate assignment request.\n");
				printf("plane id is: %d\n", msg.airplane_msg.planeId);

				// if gate is not initialized, print error
				if(ptr == NULL){
					printf("shmem not initialized\n");
					break;
				}
				sem_wait(&ptr->semaphore);
				int res = assignGate(ptr->gates, msg.airplane_msg.planeId, ptr->total_gates);
				printf("res=%d\n", res);
				status = MsgReply(rcvid, EOK, &res, sizeof(res));
				if (-1 == status) {
					perror("MsgReply");
					exit(EXIT_FAILURE);
				}
				sem_post(&ptr->semaphore);
				break;
			case AIRPLANE_CHECK:
				// receives airplane info
				printf("Received a airplane message.\n");
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

	if (-1 == munmap(ptr, sizeof(shmem_t)))
	{
		perror("munmap");
	}
   return 0;
}

int assignGate(gate* gateList, int planeId, int total_gates) {
	// for each gate check if it's empty
	for(int i=0; i<total_gates; i++)
	{
		if(gateList[i].flightId == 0) {
			gateList[i].flightId = planeId;
			printf("gate id: %d, flight id: %d\n", gateList[i].gateId, gateList[i].flightId);
			return gateList[i].gateId;
		}
	}
	printf("cannot find a gate to match!\n");
	return -1;
}
