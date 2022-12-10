/**
 * gateClient.c
 *
 * Manage gate info.
 * Act as a client to receive and response server requests.
 *
 * This client will send message of the gate number to assign to server.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <process.h>
#include <sys/dispatch.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "msg_struct.h"
#include "util.c"
#include "gateClient.h"
#include "airplaneClient.h"
#include "controlTowerServer.h"

shmem_t *ptr;

int main() {

	int coid, fd, status, ret;
	pthread_t thread0;

	// connect to server channel
	coid = name_open(ATTACH_POINT,0);
	if (-1 == coid) {
		perror("ConnectAttach");
		exit(EXIT_FAILURE);
	}

	// create shared memory for gate list
	fd = shm_open(SHMEM_NAME, O_RDWR | O_CREAT, 0666);
	if (fd == -1)
	{
		perror("shm_open()");
		exit(EXIT_FAILURE);
	}

	//set the size of the shared memory object
	ret = ftruncate(fd, sizeof(shmem_t));
	if (-1 == ret)
	{
		perror("ftruncate");
		exit(EXIT_FAILURE);
	}

	// get a pointer to a piece of the shared memory
	ptr = mmap(0, sizeof(shmem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (MAP_FAILED == ptr)
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	close(fd);

	// initialize the semaphore
	if (-1 == sem_init(&ptr->semaphore, 1, 0))
	{
		perror("sem_init");
		munmap(ptr, sizeof(shmem_t));
		shm_unlink(SHMEM_NAME);
		exit(EXIT_FAILURE);
	}

	// initialize a list of gates
	gates = malloc(TOTAL_GATES * sizeof(gate));

	for(int i = 0; i< TOTAL_GATES; ++i){
		gates[i].gateId = i + 1;
		gates[i].flightId = 0;
		printGate(&(gates[i]));
	}
	printf("gate client initialized %d gates.\n ", TOTAL_GATES);

	// copy to shared memory
	ptr->total_gates = TOTAL_GATES;

	for(int i = 0; i< TOTAL_GATES; ++i){
		ptr->gates[i] = gates[i];
		printGate(&(ptr->gates[i]));
	}

	// send a pulse to server
	status = MsgSendPulse(coid,-1,SHMEM_READY_PULSE,10);
	printf("MsgSendPulse return status: %d\n", status);

	sleep(10);

	printf("Woke up.  Now posting to the semaphore.\n");
	if (-1 == sem_post(&ptr->semaphore))
	{
		perror("sem_post");
	}

	// create the client notification thread
	pthread_create(&thread0, NULL, update_thread, NULL);

	sleep(300);

	// close everything
	if (-1 == munmap(ptr, sizeof(shmem_t)))
	{
		perror("munmap");
	}
	if (-1 == shm_unlink(SHMEM_NAME))
	{
		perror("shm_unlink");
	}
	// wait for the update thread
	void *res;
	pthread_join(thread0,&res);

	return EXIT_SUCCESS;
}

/**
 * print gate metadata.
 */
void printGate(gate* gate) {
	printf("gate id: %d\n, flight id: %d\n",
				gate->gateId,
				gate->flightId);
}

/**
 * a thread to check shared memory gate list every 60 sec.
 */
void *update_thread(void * ignore) {
	while (1) {
		sem_wait(&ptr->semaphore);
		printf("new thread for printing gates every 60 sec.\n");
		for(int i = 0; i< TOTAL_GATES; ++i){
			printGate(&(ptr->gates[i]));
		}
		sem_post(&ptr->semaphore);
		sleep(60);

	}
	return NULL;
}
