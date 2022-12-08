/**
 * msg_struct.h
 *
 * define all message structs used for IPC
 */

#ifndef _MSG_STRUCT_H_
#define _MSG_STRUCT_H_

#include <sys/iomsg.h>
#include "util.h"

#define AIRPLANE_MSG_TYPE (_IO_MAX + 1)
#define GATE_MSG_TYPE (_IO_MAX + 2)
#define PULL_PULSE_TYPE (_IO_MAX + 3)
#define REQUEST_MSG_TYPE (_IO_MAX + 4)


typedef struct
{
	uint16_t msg_type;
	airplane *plane;
} airplane_msg;


typedef struct
{
	uint16_t msg_type;
	int gateNo;
} gate_client_msg;

typedef struct
{
	uint16_t msg_type;
	int ret;
}gate_request_msg;

// all possible types of messages that can be received
typedef union
{
	uint16_t type;
	struct _pulse pulse;
	airplane_msg plane_msg;
	gate_client_msg gate_msg;
	gate_request_msg request_msg;
} my_msg_t;



#endif
