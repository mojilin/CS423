
#include "networking.h"
#include "job.h"
#include "HWMonitor.h"



void transfer_job(int fd, Job_t job)
{
	printf("Transferring job %d. Data: %f", job.id, job.data);

	channel_write(fd, JOB_TRANSFER, &job, sizeof(job));
}

int main()
{
	Job_t job;
	job.id = 10;
	job.data = 2.345;
	
	int sockfd = create_channel(PORT);
	transfer_job(sockfd, job);

	while(1);

	return 0;
}


