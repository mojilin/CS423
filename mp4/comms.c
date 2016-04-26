
#include "networking.h"
#include "job.h"
#include "HWMonitor.h"



void transfer_job(int fd, Job_t job)
{
	printf("Transferring job %d. Data: %f\n", job.id, job.data);

	channel_write(fd, JOB_TRANSFER, &job, sizeof(job));
}

int main()
{
	Job_t job;
	job.id = 10;
	job.data = 2.345;
	
	int in_data_sockfd = create_channel(LtoR_DATA_PORT);
	int in_state_sockfd = create_channel(LtoR_STATE_PORT);
	int out_state_sockfd = create_channel(RtoL_STATE_PORT);
	int out_data_sockfd = create_channel(RtoL_DATA_PORT);

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


