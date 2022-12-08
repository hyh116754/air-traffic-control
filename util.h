#ifndef _UTIL_H_
#define _UTIL_H_

#include <time.h>
#include <stdlib.h>

// define boolean values
#define MAXX      8
#define C_OK      0
#define C_NOK    -1
#define TRUE 1;
#define FALSE 0;

typedef struct {
	int id;
	int x;
	int y;
	int speed;
	int landed;
	time_t arrivalTime;
	int assignedGate;
} airplane;

typedef struct Node {
  airplane *data;
  struct Node *prev;
  struct Node *next;
} NodeType;

void addPlane(NodeType**, airplane*, int);
int deletePlane(NodeType **listHead, int id);
int  deletePlane(NodeType**, int);
void sortList(NodeType**);
void printList(NodeType*);
void cleanup(NodeType*);

#endif
