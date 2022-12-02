#include "server.h"
#include "controlTower.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>



#define MAX_STRING_LEN    256
#define ATTACH_POINT "controlSystem"
int calculate_checksum(char *text);

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

    //the server should keep receiving, processing and replying to messages
	while(1)
	{
	  //code to receive msg or pulse from client
	   rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);


	  //print the returned value of MsgReceive
	   printf("Msgreceive val: %d\n", rcvid);

	   //error checking
	   if(rcvid == -1)
		   break;

	  //check if it was a pulse
	   if(rcvid == 0){//pulse received
		   switch (msg.pulse.code) {

		   // check  if the client is gone/disconnected and print (client is gone)
		   	   case _PULSE_CODE_DISCONNECT:
		                 printf("client is disconected \n");
		                  ConnectDetach(msg.pulse.scoid);
		                  break;
		  //something else - printing code and val pulse
		   	   default:
		   		   printf("Some other pulse -> code: %d ... val: %d\n",msg.pulse.code,msg.pulse.value.sival_int);


		   }

	   }

	  // if it was a message
	   else{
		   //print received message
	       printf("server received plane: id:%d \n", msg.plane.id);

			/*calculate checksum by calling calculate_checksum
	       int checksum = calculate_checksum(msg.rmsg);

			//reply to client with checksum
	       int res = MsgReply(rcvid,0,&checksum,sizeof(checksum));

			//print the return value of MsgReply
	       printf("Msgreply val: %d\n",res);*/
	   }
	}
   //remove the name from the namespace and destroy the channel
   	   name_detach(attach,0);
   	   return 0;
}




int calculate_checksum(char *text)
{
	char *c;
	int cksum = 0;

	for (c = text; *c != '\0'; c++)
		cksum += *c;
	return cksum;
}
