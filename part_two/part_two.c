#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int *fibs;
void *runner(void *param);

int main(int argc, char *argv[]) {
	pthread_t tid;
	pthread_attr_t attr;

	if (argc != 2) {
		fprintf(stderr, "usage a.out <integer value>\n");
		return -1;
	}
	if (atoi(argv[1]) < 0) {
		fprintf(stderr, "%d must be >= 0\n", atoi(argv[1]));
		return -1;
	}

	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, runner, argv[1]);
	pthread_join(tid, NULL);

	int i;
	for (i = 0; i < atoi(argv[1]); i++) {
		printf("Fibonacci %d - %d\n", i + 1, fibs[i]);
	}

	return 0;	
}

void *runner(void *param) {
	int maxFibs = atoi(param);
	fibs = (int*) malloc(maxFibs * sizeof(int));

	if (maxFibs >= 1) {
		fibs[0] = 0;
	}
	if (maxFibs >= 2) {
		fibs[1] = 1;
	}
	int i;
	for (i = 2; i < maxFibs; i++) {
		fibs[i] = fibs[i - 2] + fibs[i - 1];
	}

	pthread_exit(0);
}
