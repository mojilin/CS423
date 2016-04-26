
#include "networking.h"
#include "job.h"
#include "HWMonitor.h"



void transfer_job(int fd, Job_t job)
{
	char temp[100];
	char buffer[100];
	printf("Transferring job %d. Data: %f\n", job.id, job.data);
	sprintf(buffer, "%d %lf", job.id , (job.data ));
	
	write(fd, buffer, 100);
	(read(fd, temp, 100));
	
}

void send_state(int fd, node_state state)
{
		
	char temp[100];
	char buffer[100];
	sprintf(buffer, "%d %f %lf", state.num_jobs, state.throttle, state.cpu_use);
	
	write(fd, buffer, 100);
//	printf("Wrote state: %s\n", buffer);
}



