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


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
volatile int planeAboutToLand; //indicator that plane needs to be assigned

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
		airplanes[i].x=0;
		airplanes[i].y=5;
		airplanes[i].speed=1;
		airplanes[i].landed = 0;

	}
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

	return 0;
}

void printPlaneInfo() {
	int i;
	for(i = 0; i< NUMPLANES; ++i){
		printf("%*s%s\n", airplanes[i].x, "", "x");
	}

}



void *planeUpdate(void *arg)
{
	while(1){
		//pass in information to print about the plane
		int i;
		pthread_mutex_lock(&mutex);
		for(i = 0; i< NUMPLANES; ++i){
			airplanes[i].x+=airplanes[i].speed;
			airplanes[i].y-=airplanes[i].speed;
			if(airplanes[i].x > MAXX ||  airplanes[i].y == 0){
						airplanes[i].landed = 1;
						planeAboutToLand = 1;
						pthread_cond_broadcast(&cond);
						printf("plane is landing...\n");
			}
		}


		printPlaneInfo();
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
		//doing work for assignment of landing gate
		for(i = 0; i< NUMPLANES; ++i){
			if(airplanes[i].landed == 1){
				printf("Found the plane to land...\n");
			}
		}

		planeAboutToLand = 0;
		pthread_cond_broadcast(&cond);
		pthread_mutex_unlock(&mutex);

	}
	return (NULL);
}







