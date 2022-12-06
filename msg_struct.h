/**
 * msg_struct.h
 *
 * define all message structs used for IPC
 */

#ifndef _MSG_STRUCT_H_
#define _MSG_STRUCT_H_

#include <sys/iomsg.h>

typedef struct
{
	uint16_t msg_type;
	airplane *plane
} airplane_msg;


typedef union
{
	struct _pulse pulse;
	airplane_msg_t message;
} airplane_msg_t;


typedef struct
{
	unit16_t msg_type;
	int gateNo;
} gate_client_msg;

#endif
