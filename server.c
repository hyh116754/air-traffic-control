#include "server.h"
#include "controlTower.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>



#define MAX_STRING_LEN    256
#define ATTACH_POINT "controlSystem"
int calculate_checksum(airplane *plane);

int main(void) {

	typedef union
	{
		struct _pulse pulse;
		airplane plane;
	} myMessage_t;

	name_attach_t *attach;
	myMessage_t msg;
    int rcvid;

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
		   break;

	  //pulse
	   if(rcvid == 0){
		   switch (msg.pulse.code) {

		   // client is gone/disconnected
		   	   case _PULSE_CODE_DISCONNECT:
		                 printf("client is disconected \n");
		                  ConnectDetach(msg.pulse.scoid);
		                  break;

		  //something else
		   	   default:
		   		   printf("Some other pulse -> code: %d ... val: %d\n",msg.pulse.code,msg.pulse.value.sival_int);


		   }

	   }

	  //message
	   else{
		   //print received message
	       printf("server received plane: id:%d \n", msg.plane.id);

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




int calculate_checksum(airplane *plane)
{

	unsigned int sum = 0;

	unsigned char *p = (unsigned char *)plane;
	for (int i=0; i<sizeof(plane); i++) {
	    sum += p[i];
	}


	return sum;
}
