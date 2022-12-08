/*
 * controlTower.c
 *
 *  Created on: Nov. 8, 2022
 *      Author: michael
 */


#include <stdio.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <stdlib.h>
#include <time.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

#include "airplaneClient.h"
#include "msg_struct.h"
#include "controlTowerServer.h"
#include "util.c"


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
volatile int planeAboutToLand; //indicator that plane needs to be assigned
NodeType *planeList;
int server_coid;
int checksum;

//main declares some threads and assigns them some function for the plane to fly in.
int main(int argc, char *argv[]) {

	//establish a connection to the server's channel
	if ((server_coid = name_open("controlSystem", 0)) == -1) {
			return EXIT_FAILURE;
	}


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
	name_close(server_coid);
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


void printPlane(const airplane *plane)
{
  printf("Plane:  id%d, x: %d y:%d speed:%d\n",plane->id,plane->x,plane->y,plane->speed);
}

/**
 * this function will send updated plane info to server as msg
 */
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

			if( currNode->data->x >= MAXX ||  currNode->data->y <= 0){
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


/**
 * this function will send a pulse to server asking for check
 * if find collision, will update server
 */
void *planeCollisionCheck(void *arg)
{
	//TODO: ensure plane collision doesn't happen through speed changes
	return (NULL);
}


/**
 * this function will send plane info to server as msg
 */
void *planeAssignment(void *arg)
{

	airplane_msg outgoing_msg;
	gate_client_msg incoming_msg;

	while(1){
		pthread_mutex_lock(&mutex);
		while (planeAboutToLand != 1)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		printf("doing assignment work...\n");
		pthread_mutex_lock(&mutex);

		//we are finding the airplane in the current "in air" list which has landed and remove it from air list
		//we need to pass it over server communication to our gate somehow
		//TODO: after we add it to a gate list

		int ret;
		NodeType *currNode;
	    currNode = planeList;
	    while(currNode != NULL){

	    	if(currNode->data->landed == 1){
				printf("Found the plane to land...\n");

	    		ret = deletePlane(&planeList,currNode->data->id);

	    		//send msg type: airplane_msg
	    		//receive msg type: gate_client_msg
	    		outgoing_msg.msg_type = AIRPLANE_MSG_TYPE;
	    		outgoing_msg.plane = currNode->data;

				MsgSend(server_coid, &outgoing_msg, sizeof(outgoing_msg),
							&incoming_msg, sizeof(incoming_msg));
				printf("Received gate number is %d \n",incoming_msg.gateNo);

				// TODO: update the plane with the gate number
				currNode->data->assignedGate = incoming_msg.gateNo;

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







