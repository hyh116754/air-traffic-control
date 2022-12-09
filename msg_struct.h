/**
 * msg_struct.h
 *
 * define all message structs used for IPC
 */

#ifndef _MSG_STRUCT_H_
#define _MSG_STRUCT_H_

#include <sys/iomsg.h>
#include "util.h"
#include "gateClient.h"

#define AIRPLANE_MSG_TYPE (_IO_MAX + 1)
#define AIRPLANE_REQUEST_GATE (_IO_MAX + 2)
#define GATE_UPDATE_MSG (_IO_MAX + 3)
#define GATE_LIST_MSG (_IO_MAX + 4)


typedef struct
{
	uint16_t msg_type;
	airplane *plane;
} airplane_msg;

typedef struct
{
	uint16_t msg_type;
	gate* gateList;
}gate_list_msg;

// all possible types of messages that can be received
typedef union
{
	uint16_t type;
	struct _pulse pulse;
	airplane_msg plane_msg;
	gate_list_msg gate_list_msg;
} my_msg_t;



#endif
