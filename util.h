#ifndef _UTIL_H_
#define _UTIL_H_

#include <time.h>
#include "airplaneClient.h"

// define boolean values
#define TRUE 1;
#define FALSE 0;

typedef struct Node {
  airplane *data;
  struct Node *prev;
  struct Node *next;
} NodeType;


void initPlane(int,int,int,int,int,airplane**);
void addPlane(NodeType**, airplane*, int);
int deletePlane(NodeType **listHead, int id);
int  deletePlane(NodeType**, int);
void sortList(NodeType**);
void printList(NodeType*);
void cleanup(NodeType*);

#endif
