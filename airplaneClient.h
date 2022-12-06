#ifndef _AIRPLANECLIENT_H_
#define _AIRPLANECLIENT_H_

#include <time.h>

#define NUMPLANES      10
#define MAXX      8
#define C_OK      0
#define C_NOK    -1

typedef union {
	int id;
	int x;
	int y;
	int speed;
	int landed;
	//added
	coordinate* position;
	arrivalInfo* arrival;
} airplane;

typedef struct {
	int x;
	int y;
} coordinate;

typedef struct {
	time_t arrival_time
	int assignedGate;
} arrivalInfo;


airplane airplanes[10];


//functions for flight control
void printPlane(const airplane*);
void *planeUpdate(void *arg);
void *planeCollisionCheck(void *arg);
void *planeAssignment(void *arg);


#endif
