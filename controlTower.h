#define NUMPLANES      10
#define MAXX      8
#define C_OK      0
#define C_NOK    -1

typedef struct{
	int id;
	int x;
	int y;
	int speed;
	int landed;
}airplane;


typedef struct Node {
  airplane *data;
  struct Node *prev;
  struct Node *next;
} NodeType;


airplane airplanes[10];


//functions for linked list
void initPlane(int,int,int,int,int,airplane**);
void addPlane(NodeType**, airplane*, int);
int deletePlane(NodeType **listHead, int id);
int  deletePlane(NodeType**, int);
void printList(NodeType*);
void printPlane(const airplane*);
void cleanup(NodeType*);


void *planeUpdate(void *arg);
void *planeCollisionCheck(void *arg);
void *planeAssignment(void *arg);



typedef struct{

}gate;
