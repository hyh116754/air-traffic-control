/*
 *  controlTowerServer.h
 *  Includes the constants and struct definition for a server.
 *
 */

#ifndef _CONTROL_TOWER_SERVER_H_
#define _CONTROL_TOWER_SERVER_H_

#include "util.h"
#include "gateClient.h"

#define MAX_STRING_LEN 256
#define CKSUM_MSG_TYPE (_IO_MAX + 1)
#define ATTACH_POINT "controlSystem"

int getGateToAssign(gate* gateList, int planeId, int total_gates);

#endif
