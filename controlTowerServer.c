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


#define MAX_STRING_LEN    256


int main(void) {

	name_attach_t *attach;
	my_msg_t msg;
    int rcvid, status, checksum;

	//creating a channel
   if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
	   return EXIT_FAILURE;
   }

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
		//pulse
		if(rcvid == 0){
		   switch (msg.pulse.code) {
			   case 1:
			   // pulse code 1: plane about to land

			   case _PULSE_CODE_DISCONNECT:
						 printf("client is disconnected \n");
						  ConnectDetach(msg.pulse.scoid);
						  break;
			   default:
				   printf("Cannot resolve pulse -> code: %d ... val: %d\n",msg.pulse.code,msg.pulse.value.sival_int);
		   }
		}

	  //receive a message
	   else if (rcvid > 0){
		   //print received message
	       printf("server received a message.");

		   switch (msg.type) {
		   case AIRPLANE_MSG_TYPE:
			   printf("Received a airplane message.\n");

			   break;
		   case GATE_MSG_TYPE:
			   printf("Received a gate message.\n");
		       int res = MsgReply(rcvid,0,&checksum,sizeof(checksum));
		       printf("Msgreply val: %d\n",res);
			   break;
		   }
	   }
	}
   //remove the name from the namespace and destroy the channel
   name_detach(attach,0);
   return 0;
}
