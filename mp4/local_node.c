
#include "networking.h"
#include "job.h"
#include "HWMonitor.h"
#include <pthread.h>



#define NBYTES 200

char buffer[NBYTES];
int sockfd;


void * comm_read_thread(void * arg)
{
	Job_t job;	
	n_state state;
	while(1)
	{

		state = channel_read(sockfd, buffer, NBYTES);
		puts("SOMETHING RECEIVED");
		if(state == JOB_TRANSFER)
		{
			sscanf(buffer, "id: %d, data: %lf", &(job.id), &(job.data));
			printf("Job transferred id = %d, data = %f", job.id, job.data);
		}

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
