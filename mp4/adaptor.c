#include "adaptor.h"
#include "HWMonitor.h"
#include "comms.h"
#include "job_queue.h"

#define THRESHOLD 420
#define SENDER_I 
// #define RECEIVER_I 
// #define SYM_I 

extern int out_state_sockfd;

void * adaptor_thread(void * arg) {
	adaptor_arg_t * args = (adaptor_arg_t *)arg;

	while(420) {
		// queue length based on transfer policy
		// next available job for selection policy, since jobs are identical it shouldn't matter much

		// sender initiated
		#ifdef SENDER_I

		if ((args->remote_state->num_jobs - args->local_state->num_jobs) > THRESHOLD) {
			if (!isEmpty()) {
				Job_t job = dequeue();

				transfer_job(out_state_sockfd, job);
			} else {
				// no more available jobs
				continue;
			}
		}
		
		#endif
	}
}