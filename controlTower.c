/*
 * planeThreads.cpp
 *
 *  Created on: Nov. 8, 2022
 *      Author: michael
 */

#include "controlTower.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <time.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
volatile int planeAboutToLand; //indicator that plane needs to be assigned
NodeType *planeList;

//main declares some threads and assigns them some function for the plane to fly in.
int main(int argc, char *argv[]) {

	srand(time(NULL));
	pthread_t thread0;
	pthread_t thread1;
	pthread_t thread2;
	pthread_attr_t attr;
	airplane *newAirplane;

	//creating linked list of airplane objects
	int i;
	for(i = 0; i< NUMPLANES; ++i){
		initPlane(i,0,(rand() % 10)+2,(rand() % 5)+1,0,&newAirplane);
		addPlane(&planeList,newAirplane,i);
	}
	printList(planeList);
	planeAboutToLand = 0;

	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_RR);

	//thread for plane updating
	struct sched_param schedparam;
	schedparam.sched_priority = 5;
	pthread_attr_setschedparam(&attr, &schedparam);
	pthread_create(&thread0, &attr, planeUpdate, NULL);


	//thread for plane assignment to gates
	schedparam.sched_priority = 6;
	pthread_attr_setschedparam(&attr, &schedparam);
	pthread_create(&thread1, &attr, planeAssignment, NULL);

	//thread to check for collisions
	schedparam.sched_priority = 7;
	pthread_attr_setschedparam(&attr, &schedparam);
	pthread_create(&thread2, &attr, planeCollisionCheck, NULL);

	sleep(6);
	pthread_attr_destroy(&attr);
	void *ret;
	pthread_join(thread0,&ret);
	pthread_join(thread1,&ret);
	pthread_join(thread2,&ret);

	cleanup(planeList);
	return 0;
}

//dynamic memory allocation for a plane object
void initPlane(int id ,int x,int y,int speed,int landed,airplane **plane)
{
  *plane = malloc(sizeof(airplane));
  (*plane)->id = id;
  (*plane)->x = x;
  (*plane)->y = y;
  (*plane)->speed = speed;
  (*plane)->landed = landed;
}

//linked list memory deallocation
void cleanup(NodeType *listHead)
{
  NodeType *currNode;
  NodeType *nextNode;
  currNode = listHead;
  while (currNode != NULL) {
    nextNode = currNode->next;
    free(currNode->data);
    free(currNode);
    currNode = nextNode;
  }
}

void addPlane(NodeType **listHead, airplane *plane, int pos)
{
  NodeType *currNode;
  NodeType *prevNode;
  NodeType *newNode;
  int currPos = 0;

  currNode = *listHead;
  prevNode = NULL;

  while (currNode != NULL) {
    if (currPos == pos)
      break;
    ++currPos;
    prevNode = currNode;
    currNode = currNode->next;
  }

  if (currPos != pos) {
    printf("Error:  invalid position\n");
    free(plane);
    return;
  }

  newNode = malloc(sizeof(NodeType));
  newNode->data = plane;
  newNode->prev = NULL;
  newNode->next = NULL;

  if (prevNode == NULL)
    *listHead = newNode;
  else
    prevNode->next = newNode;

  newNode->next = currNode;

  newNode->prev = prevNode;

  if (currNode != NULL)
    currNode->prev = newNode;
}



int deletePlane(NodeType **listHead, int id)
{
  NodeType *currNode;
  NodeType *prevNode;

  currNode = *listHead;
  prevNode = NULL;

  while (currNode != NULL) {
    if (currNode->data->id == id)
      break;

    prevNode = currNode;
    currNode = currNode->next;
  }

  if (currNode == NULL) {
    return C_NOK;
  }

  if (prevNode == NULL)
    *listHead = currNode->next;
  else
    prevNode->next = currNode->next;

  if (currNode->next != NULL)
    currNode->next->prev = prevNode;

  free(currNode->data);
  free(currNode);

  return C_OK;

}


void printList(NodeType *listHead)
{
  NodeType *currNode = listHead;
  while (currNode != NULL) {
    printPlane(currNode->data);
    currNode = currNode->next;
  }

}

void printPlane(const airplane *plane)
{
  printf("Plane:  id%d, x: %d y:%d speed:%d\n",plane->id,plane->x,plane->y,plane->speed);
}


void *planeUpdate(void *arg)
{
	while(1){
		//pass in information to print about the plane
		pthread_mutex_lock(&mutex);
		NodeType *currNode;
		currNode = planeList;

		while(currNode != NULL){
			currNode->data->x += currNode->data->speed;
			currNode->data->y -= currNode->data->speed;

			if( currNode->data->x > MAXX ||  currNode->data->y == 0){
			   currNode->data->landed = 1;
			   planeAboutToLand = 1;
			   pthread_cond_broadcast(&cond);
			   printf("plane is landing...\n");
			}

			currNode = currNode->next;
		}


		pthread_mutex_unlock(&mutex);
		sleep(3);
	}

	return (NULL);

}

void *planeCollisionCheck(void *arg)
{
	//pass in information to print about the plane
	return (NULL);
}



void *planeAssignment(void *arg)
{
	while(1){
		pthread_mutex_lock(&mutex);
		while (planeAboutToLand != 1)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		printf("doing assignment work...\n");
		int i;
		pthread_mutex_lock(&mutex);

		//need to find the airplane in the current "in air" list and remove it
		//TODO: after we add it to a gate list

		int ret;
		NodeType *currNode;
	    currNode = planeList;
	    while(currNode != NULL){

	    	if(currNode->data->landed == 1){
				printf("Found the plane to land...\n");

	    		ret = deletePlane(&planeList,currNode->data->id);
				if(ret < 0)
					printf("error deleting plane..\n");
				else
					printf("deleted plane from list\n");\
				break;
	    	}


	    	currNode = currNode->next;
	    }
	    printList(planeList);

		planeAboutToLand = 0;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);

	}
	return (NULL);
}







