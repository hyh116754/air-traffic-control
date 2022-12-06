/**
 * controlTowerServer.c
 *
 * Server in the control tower for communication.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/iomsg.h>

#include "controlTowerServer.h"
#include "airplaneClient.h"
#include "gateClient.h"
#include "util.h"
$include "msg_struct.h"


#define MAX_STRING_LEN    256


int main(void) {

	name_attach_t *attach;
	myMessage_t msg;
    int rcvid, status, checksum;

	//creating a channel
   if ((attach = name_attach(NULL, ATTACH_POINT, 0)) == NULL) {
	   return EXIT_FAILURE;
   }

	while(1)
	{
	  //code to receive msg or pulse from client
	   rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);

	   //error checking
	   if(rcvid == -1)
		   perror("MsgReceive");
	   	   exit(EXIT_FAILURE);

	  //pulse
	   if(rcvid == 0){
		   switch (msg.pulse.code) {
		   	   case 1:
			   // pulse code 1: plane about to land

		   	   case 2:
			   // pulse code 2: plane request for gate assignment

		   	   // client is gone/disconnected
		   	   case _PULSE_CODE_DISCONNECT:
		                 printf("client is disconnected \n");
		                  ConnectDetach(msg.pulse.scoid);
		                  break;
		   	   default:
		   		   printf("Cannot resolve pulse -> code: %d ... val: %d\n",msg.pulse.code,msg.pulse.value.sival_int);
		   }

	   }

	  //receive a message
	   else{
		   //print received message
	       printf("server received a message from airplane id:%d \n", msg.plane.id);

	       //switch case on pulse code

	       //carrying out data integrity verification using checksum
	       int checksum = calculate_checksum(&msg.plane);
	       int res = MsgReply(rcvid,0,&checksum,sizeof(checksum));
	       printf("Msgreply val: %d\n",res);
	   }
	}
   //remove the name from the namespace and destroy the channel
   name_detach(attach,0);
   return 0;
}
