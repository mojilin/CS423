#include <unistd.h>
#include <stdio.h>
#include "WorkThread.h"
#include "job.h"
#include "job_queue.h"

#define EMPTY_SLEEP_TIME 200 //microseconds

double result[A_SIZE];

extern float throttle_in;


void * worker_thread(void *arg) {

	int counter = 0;
	while (1) {
		// if queue is not empty
		if (!isEmpty()) {

			if(counter >throttle_in * 5000)
			{
				usleep(100000 * (1 - throttle_in));
				counter = 0;
			}
			int i = 0;
			Job_t job = dequeue();

			if (job.id == -1)
				continue;

			printf("WORKER THREAD: Processing job %d\n", job.id);

			/* This job takes 20 microseconds */
			for (i = 0; i < 6000; i++) {
				job.data += 1.111111;
			}


			/* On finish, add it to the result array */
			result[job.id] = job.data;
			printf("COMPUTED job %d\n", job.id);
			counter++;

		} else {
			counter = 0;
			usleep(EMPTY_SLEEP_TIME);
		}
	}

	return NULL;
}

