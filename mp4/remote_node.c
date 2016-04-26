#include "comms.h"
#include "job.h"
#include "job_queue.h"
#include "networking.h"
#include "HWMonitor.h"
#include "WorkThread.h"
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>

#define NBYTES 100
#define STATE_TIMER 100000

int in_data_sockfd, out_data_sockfd, in_state_sockfd, out_state_sockfd;

node_state remote_state;
node_state local_state;
double throttle_in;
extern double result[A_SIZE];

void * comm_read_thread(void * arg)
{
	Job_t job;	
	
	char buffer[NBYTES];
	char temp[NBYTES];
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


void * state_read_thread(void * arg)
{
	char temp[NBYTES];
	char buffer[NBYTES];
	
	while(1)
	{

	    read(in_state_sockfd, buffer, NBYTES);
		sscanf(buffer, "%d %f %lf", &remote_state.num_jobs, &remote_state.throttle,
			   		&remote_state.cpu_use);
		printf("STATE RECEIVED num_jobs = %d, cpu_use = %lf, throttle = %f\n", remote_state.num_jobs, remote_state.cpu_use, remote_state.throttle);
		
		sprintf(temp, "ACK");
		write(in_state_sockfd, temp, NBYTES);
		
	}
}

void timer_handler (int signum)
{
	local_state.num_jobs = get_queue_size();
	local_state.cpu_use = get_cpu_use();
	local_state.throttle = throttle_in;
	printf("TIMER! Queue Size = %d\n", local_state.num_jobs);

	send_state(out_state_sockfd, local_state);
		
	
}

int main(int argc, char ** argv)
{
	Job_t job;
	job.id = 10;
	job.data = 2.345;
	
	pthread_t read_thread, worker_td, state_r_td;

	arg_t worker_arg;
	worker_arg.throttle = 0;

	struct sigaction sa;
	struct itimerval timer;
	/* Install the timer handler */
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = timer_handler;
	sigaction(SIGALRM, &sa, NULL);

	in_data_sockfd = create_channel(LtoR_DATA_PORT);
	in_state_sockfd = create_channel(LtoR_STATE_PORT);
	out_state_sockfd = create_channel(RtoL_STATE_PORT);
	out_data_sockfd = create_channel(RtoL_DATA_PORT);

	/* SET Timer to send state every STATE_TIMER microseconds */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = STATE_TIMER;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = STATE_TIMER;
	setitimer(ITIMER_REAL, &timer, NULL);

	pthread_create(&state_r_td, NULL, state_read_thread, 0);
	pthread_create(&read_thread, NULL, comm_read_thread, 0);
	pthread_create(&worker_td, NULL, worker_thread, &worker_arg);

	while(1)
	{
		getc(stdin);
		transfer_job(out_data_sockfd, job);
	}

	pthread_join(read_thread, NULL);

	close(out_data_sockfd);
	close(out_state_sockfd);
	close(in_data_sockfd);
	close(in_state_sockfd);
	return 0;
}


