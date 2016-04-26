
#include "networking.h"
#include "job.h"
#include "job_queue.h"
#include "HWMonitor.h"
#include "WorkThread.h"
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>



#define NBYTES 200
#define STATE_TIMER 100000

char buffer[NBYTES];
int in_data_sockfd, in_state_sockfd, out_data_sockfd, out_state_sockfd;
char temp[NBYTES];

extern double result[A_SIZE];

node_state local_state;


void * comm_read_thread(void * arg)
{
	Job_t job;	
	
	while(1)
	{

	    read(in_data_sockfd, buffer, NBYTES);
		sscanf(buffer, "%d %lf", &(job.id), &(job.data));
		printf("JOB RECEIVED id = %d, Job data = %lf\n", job.id, job.data);
		
		sprintf(temp, "ACK %d", job.id);
		write(in_data_sockfd, temp, NBYTES);
		enqueue(job);
		
	}
}


void timer_handler (int signum)
{
	
}

int main()
{
	pthread_t read_thread, worker_td;
	
	arg_t worker_arg;
	worker_arg.throttle = 0;
	int i;

	struct sigaction sa;
	struct itimerval timer;
	/* Install the timer handler */
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = timer_handler;
	sigaction(SIGVTALRM, &sa, NULL);


	/* Initialize result array */
	for(i = 0; i < A_SIZE; i++)
	{
		result[i] = -1.0f;
	}
	

	/* Initialize the incoming socket*/
	in_data_sockfd = join_channel(ip_address, RtoL_DATA_PORT);
	in_state_sockfd = join_channel(ip_address, RtoL_STATE_PORT); 
	out_data_sockfd = join_channel(ip_address, LtoR_DATA_PORT);
	out_state_sockfd = join_channel(ip_address, LtoR_STATE_PORT); 

	/* SET Timer to send state every STATE_TIMER microseconds */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = STATE_TIMER;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = STATE_TIMER;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);

	/* Create the threads */
	pthread_create(&read_thread, NULL, comm_read_thread, 0);
	pthread_create(&worker_td, NULL, worker_thread, &worker_arg);

	pthread_join(read_thread, NULL);

	close(in_data_sockfd);
	close(in_state_sockfd);

	return 0;
}
