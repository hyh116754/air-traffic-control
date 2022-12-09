/**
 * msg_struct.h
 *
 * define all message structs used for IPC
 */

#ifndef _MSG_STRUCT_H_
#define _MSG_STRUCT_H_

#include <sys/iomsg.h>
#include <semaphore.h>
#include "util.h"
#include "gateClient.h"

#define AIRPLANE_CHECK (_IO_MAX + 1)
#define AIRPLANE_GATE_REQUEST (_IO_MAX + 2)
#define GATE_LIST_MSG (_IO_MAX + 4)
#define SHMEM_READY_PULSE (_PULSE_CODE_MINAVAIL + 10)
#define SHMEM_NAME "/testing"
#define MAX_GATE_NUM 20

typedef struct
{
	uint16_t msg_type;
	int planeId;
} airplane_msg;

// all possible types of messages that can be received
typedef union
{
	uint16_t type;
	struct _pulse pulse;
	airplane_msg airplane_msg;
} my_msg_t;

typedef struct
{
	sem_t semaphore;
	int total_gates;
	gate gates[MAX_GATE_NUM];
} shmem_t;


#endif
