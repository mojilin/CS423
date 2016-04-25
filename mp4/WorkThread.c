#include <unistd.h>
#include "WorkThread.h"
#include "job_queue.h"

#define EMPTY_SLEEP_TIME 200 //microseconds

void compute(Job_t job);

void worker_thread(void *arg) {
	arg_t * args = (arg_t *)arg;

	while (1) {
		// if queue is not empty
		if (!isEmpty()) {
			int i = 0;
			int j = 0;
			Job_t job = dequeue();

			if (job.id == -1)
				continue;

			for (i = 0; i < 6000; i++) {
				job.data += 1.111111;
			}

			// what do when done
		} else {
			usleep(EMPTY_SLEEP_TIME);
		}
	}
}

void compute(Job_t job) {

}