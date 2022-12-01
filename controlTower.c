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


//main declares some threads and assigns them some function for the plane to fly in.
int main(int argc, char *argv[]) {


	pthread_t thread0;
	pthread_t thread1;
	pthread_t thread2;
	pthread_attr_t attr;


	//creating array of airplane objects
	int i;
	for(i = 0; i< NUMPLANES; ++i){
		airplanes[i].id = i;
		airplanes[i].x=5;
		airplanes[i].y=10;
		airplanes[i].speed=1;
		airplanes[i].landed = 0;

	}

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



	pthread_attr_destroy(&attr);
	void *ret;
	pthread_join(thread0,&ret);
	pthread_join(thread1,&ret);
	pthread_join(thread2,&ret);

	return 0;
}

void printPlaneInfo() {
	//needed structs for get sched param
	struct sched_param param;
	int policy;

	//sleep for 2 seconds
	sleep(2);

	pid_t tid = gettid();
	pthread_getschedparam (tid, &policy, &param);


	int priority = param.sched_priority;
	printf("The plane id is: %d and the thread priority is: %d \n",tid,priority);
	printf("Plane location is at:...");

}



void *planeUpdate(void *arg)
{
	//pass in information to print about the plane
	int i;
	for(i = 0; i< NUMPLANES; ++i){
			airplanes[i].id = i;
			airplanes[i].x+=airplanes[i].speed;
			airplanes[i].y+=airplanes[i].speed;
			printf("%*s%s\n", airplanes[i].x, "", "x");
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
	return (NULL);
}







