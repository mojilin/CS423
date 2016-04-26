#include "adaptor.h"
#include "HWMonitor.h"
#include "comms.h"
#include "job_queue.h"

#define THRESHOLD 420

extern int out_data_sockfd;
extern double A[A_SIZE];
extern int A_count;

void * adaptor_thread(void * arg) {
	adaptor_arg_t * args = (adaptor_arg_t *)arg;

	while(420) {
		#ifdef LOCAL
		Job_t newJob = {A_count, A[A_count]};
		A_count++;

		// sender initiate
		if (isFull() && args->remote_state->num_jobs < QUEUE_MAX) {
			transfer_job(out_data_sockfd, newJob);
		} else {
			enqueue(newJob);
		}
		#endif
		// queue length based on transfer policy
		// next available job for selection policy, since jobs are identical it shouldn't matter much

		if ((args->local_state->num_jobs - args->remote_state->num_jobs) > THRESHOLD) {
			if (!isEmpty()) {
				Job_t job = dequeue();

				transfer_job(out_data_sockfd, job);
			} else {
				// no more available jobs
				continue;
			}
		}
	}
}
