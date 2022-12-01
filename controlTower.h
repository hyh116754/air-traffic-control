#define NUMPLANES      10
#define MAXX      8


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


void initPlane(int,int,int,int,int,airplane**);
void addPlane(NodeType**, airplane*, int);
int  deletePlane(NodeType**, char*);
void printList(NodeType*);
void printPlane(const airplane*);
void cleanup(NodeType*);


void *planeUpdate(void *arg);
void *planeCollisionCheck(void *arg);
void *planeAssignment(void *arg);
void printPlaneInfo();



typedef struct{

}gate;
