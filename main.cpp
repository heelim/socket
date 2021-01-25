
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "Queue.h"
pthread_mutex_t mutex_lock;
int temp;
Queue a(2);
Queue b(2);
int turn=0;
void thread_function(int id) {	
	int val;
	while(temp > 1) {
		if(turn == id){
			printf("thread %d running\n", id);
			pthread_mutex_lock(&mutex_lock);
			val = b.dequeue();
			a.enqueue(val);
			val += a.dequeue();
			b.enqueue(val);	
			temp--;
			turn=(id+1)%2;
			pthread_mutex_unlock(&mutex_lock);
		}
	}	
}

int main(int argc, char *argv[]) {
	if(argc<2) {
		printf("usage %s (natural number)\n", argv[0]);
		return 1;
	}
	pthread_t * threads;
	pthread_mutex_t mutex_lock;
    pthread_mutex_init(&mutex_lock, NULL);
    
    int i;
	int num = atoi(argv[1]);
	temp = num;
	a.enqueue(0);
	b.enqueue(1);
	printf("calculating #%d number in fibonacci seq\n", temp);

	threads = (pthread_t *) malloc(sizeof(pthread_t) * 2);

	for (i = 0; i < 2; i++)
		pthread_create(&threads[i], NULL, (void *(*) (void *)) thread_function, (void *) i);

	for (i = 0; i < 2; i++)
		pthread_join(threads[i], NULL);

	free(threads);
	printf("calculated: %d\n", b.dequeue());

	return 0;
}