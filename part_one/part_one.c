#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

int SharedVariable = 0;
pthread_mutex_t lock;
pthread_barrier_t barrier;

void SimpleThread(int which) {
	int num, val;
	for (num = 0; num < 20; num++) {
		if (random() > RAND_MAX/2) {
			usleep(10);
		}
		pthread_mutex_lock(&lock);
		val = SharedVariable;
		printf("*** thread %d sees value %d\n", which, val);
		SharedVariable = val + 1;
		pthread_mutex_unlock(&lock);
	}
	pthread_barrier_wait(&barrier);
	val = SharedVariable;
	printf("Thread %d sees final value %d\n", which, val);
}

void *SimpleThreadStart(void *id_ptr) {
	int *id_ptr_local = (int *)id_ptr;
	SimpleThread(*id_ptr_local);
}

int main(int argc, char** argv) {	
	if (argc <= 1) {
		printf("Thread count command line parameter required!\n");
		exit(0);
	}
	if (!isdigit((unsigned char)*argv[1])) {
		printf("Thread count must be number!\n");
		exit(0);
	}

	int thread_count = atoi(argv[1]); 
	if (thread_count < 0) {
		printf("Thread count must be a postive integer!\n");
		exit(0);
	}
	
	int i;
	int ids[thread_count];
	pthread_t tid[thread_count];
	pthread_barrier_init(&barrier, NULL, thread_count);
	for (i = 0; i < thread_count; i++) {
		ids[i] = i;
		pthread_create(&tid[i], NULL, SimpleThreadStart, (void *)&ids[i]);
	}
	for (i = 0; i < thread_count; i++) {
		pthread_join(tid[i], NULL);
	}
	pthread_barrier_destroy(&barrier);	
	exit(0);
}
