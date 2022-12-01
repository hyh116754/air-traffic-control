#define NUMPLANES      10


typedef struct{
	int id;
	int x;
	int y;
	int speed;
	int landed;
}airplane;


typedef struct{

}gate;
airplane airplanes[10];


void *planeUpdate(void *arg);
void *planeCollisionCheck(void *arg);
void *planeAssignment(void *arg);
void printPlaneInfo();


