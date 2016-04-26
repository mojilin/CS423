
#include "networking.h"
#include "job.h"
#include "job_queue.h"
#include "HWMonitor.h"
#include <pthread.h>



#define NBYTES 200

char buffer[NBYTES];
int sockfd;


void * comm_read_thread(void * arg)
{
	Job_t job;	
	
	while(1)
	{

	    read(sockfd, buffer, NBYTES);
		sscanf(buffer, "%d %lf", &(job.id), &(job.data));
		printf("JOB RECEIVED id = %d, Job data = %lf\n", job.id, job.data);
		
		enqueue(job);
		
	}
}

int main()
{
	pthread_t read_thread;

	sockfd = join_channel(ip_address, PORT);

	pthread_create(&read_thread, NULL, comm_read_thread, 0);


	pthread_join(read_thread, NULL);


	return 0;
}
