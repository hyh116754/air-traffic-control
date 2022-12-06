#ifndef _AIRPLANECLIENT_H_
#define _AIRPLANECLIENT_H_


#define NUMPLANES      10
#define MAXX      8
#define C_OK      0
#define C_NOK    -1



typedef struct {
	int id;
	int x;
	int y;
	int speed;
	int landed;
	//added
	coordinate* position;
	arrival* arrivalInfo;
} airplane;

typedef struct {
	int x;
	int y;
} coordinate;


typedef struct arrival
{
	int flightCode;
	int arrivalTime;
	int assignedGate;
};


airplane airplanes[10];


//functions for flight control
void printPlane(const airplane*);
void *planeUpdate(void *arg);
void *planeCollisionCheck(void *arg);
void *planeAssignment(void *arg);


#endif
