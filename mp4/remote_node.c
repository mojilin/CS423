#include "comms.h"
#include "job.h"
#include "job_queue.h"
#include "networking.h"
#include <pthread.h>

#define NBYTES 100

int in_data_sockfd, out_data_sockfd, in_state_sockfd, out_state_sockfd;
node_state local_state;

void * state_read_thread(void * arg)
{
	Job_t job;	
	char temp[NBYTES];
	char buffer[NBYTES];
	
	while(1)
	{

	    read(in_state_sockfd, buffer, NBYTES);
		sscanf(buffer, "%d %f %lf", &local_state.num_jobs, &local_state.throttle,
			   		&local_state.cpu_use);
		printf("STATE RECEIVED num_jobs = %d, cpu_use = %lf, throttle = %f\n", local_state.num_jobs, local_state.cpu_use, local_state.throttle);
		
		sprintf(temp, "ACK");
		write(in_state_sockfd, temp, NBYTES);
		enqueue(job);
		
	}
}


int main(int argc, char ** argv)
{
	Job_t job;
	job.id = 10;
	job.data = 2.345;
	
	pthread_t state_r_td;

	in_data_sockfd = create_channel(LtoR_DATA_PORT);
	in_state_sockfd = create_channel(LtoR_STATE_PORT);
	out_state_sockfd = create_channel(RtoL_STATE_PORT);
	out_data_sockfd = create_channel(RtoL_DATA_PORT);

	pthread_create(&state_r_td, NULL, state_read_thread, 0);

	while(1)
	{
		getc(stdin);
		transfer_job(out_data_sockfd, job);
	}

	close(out_data_sockfd);
	close(out_state_sockfd);
	close(in_data_sockfd);
	close(in_state_sockfd);
	return 0;
}


