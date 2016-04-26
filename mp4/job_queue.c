#include "job_queue.h"
#include "job.h"
#include <pthread.h>
#include <stdio.h>

static Job_t queue[QUEUE_MAX];
static int tail = -1;
static int head = -1;

static int job_queue_size = 0;

static pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;

const static Job_t empty_job = {-1, -1.0};

void clear() {
	pthread_mutex_lock(&queue_mutex);
	head = -1;
	tail = -1;
	pthread_mutex_unlock(&queue_mutex);
}

int enqueue(Job_t job) {
	pthread_mutex_lock(&queue_mutex);
	if (isFull())
		return -1;

	tail++;
	job_queue_size++;
	queue[tail % QUEUE_MAX] = job;
	printf("ENQUEUED job, id: %d, data: %lf\n", job.id, job.data);
	pthread_mutex_unlock(&queue_mutex);
	return 0;
}

Job_t dequeue() {
	Job_t retval;

	pthread_mutex_lock(&queue_mutex);
	if (isEmpty()) {
		pthread_mutex_unlock(&queue_mutex);

		return empty_job;
	} else {
		head++;
		job_queue_size--;
		retval = queue[head % QUEUE_MAX];

		pthread_mutex_unlock(&queue_mutex);
		return retval;
	} 
}

int isEmpty() {
	return (head == tail);
}

int isFull() {
	return ((tail - QUEUE_MAX) == head);
}

int get_queue_size()
{
	return job_queue_size;
}
