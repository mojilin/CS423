
#include "networking.h"
#include "job.h"
#include "HWMonitor.h"

char buffer[100];

int i =0 ;
void transfer_job(int fd, Job_t job)
{
	char temp[100];
	printf("Transferring job %d. Data: %f\n", job.id + i, job.data + i);
	sprintf(buffer, "%d %lf", job.id + i, (job.data + i));
	i++;
	
	write(fd, buffer, 100);
	while(read(fd, temp, 100) <= 0);
	printf("Message Acknoledged: %s\n", temp);
	read(fd,temp,100);
	
}

int main()
{
	Job_t job;
	job.id = 10;
	job.data = 2.345;
	
	int j;
	int sockfd = create_channel(PORT);
	for( j =0 ; j < 100; j++)
	{
		transfer_job(sockfd, job);
	}

	while(1);
	close(sockfd);
	return 0;
}


