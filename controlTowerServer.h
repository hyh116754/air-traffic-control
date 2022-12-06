/*
 *  controlTowerServer.h
 *  Includes the constants and struct definition for a server.
 *
 */

#ifndef _CONTROL_TOWER_SERVER_H_
#define _CONTROL_TOWER_SERVER_H_

#include <sys/iomsg.h>

#define MAX_STRING_LEN 256
#define CKSUM_MSG_TYPE (_IO_MAX + 1)
#define ATTACH_POINT "controlSystem"



typedef struct
{
	uint16_t msg_type;
	airplane *plane
} airplane_msg_t;

// define a union with pulse
typedef union
{
	struct _pulse pulse;
	airplane_msg_t message;
} myMessage_t;


#endif
