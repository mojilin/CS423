#include "userapp.h"


const char proc_filename[] =  "/proc/mp2/status";


int main(int argc, char* argv[])
{
	FILE* handle;
	int thePid = (int) getpid();
	int period = 1000;	/* Period of the job in milliseconds*/
	int computation = 20; /* Processing time of job in milliseconds*/
	int n = 32;
	char tempstring[50]; /* temp string */
	int i = 0;
	int num_jobs = 10;
	int tempPID;
	struct timeval t0;


	if(argc == 3)
	{
		period = strtol(argv[1], NULL, 10);
		num_jobs = strtol(argv[2], NULL, 10);
	}
	handle = fopen(proc_filename ,"r+");
	if(handle == NULL)
	{
		fprintf(stderr, "Could not open file\n");
		return -1;
	}
	/* Registration */
	fprintf(handle, "R, %d, %d, %d", thePid, period, computation);
	fclose(handle);

	/* Read status to check if registered */
	handle = fopen(proc_filename ,"r+");
	/* Read into a string until a newline, then check if the PID matches
	 * current PID
	 */
	while(1)
	{
		if(feof(handle))
		{
			printf("FILE registration failed\n");
			return 1;
		}
		tempstring[i] = fgetc(handle);
		if (tempstring[i] == EOF)
		{
			fclose(handle);
			handle = fopen(proc_filename ,"r+");
		}
		if(tempstring[i] == '\n' || tempstring[i] == '\0')
		{
				i = 0;
				sscanf(tempstring, "%*s %d", &tempPID);
				if(thePid == tempPID)
				{
					printf("Registration successful\n");
					break;
				}
		}
		else
			i++;


	}
	fclose(handle);

	gettimeofday(&t0, NULL);
	/* YIELD */
	handle = fopen(proc_filename ,"r+");
	fprintf(handle, "Y, %d", thePid);
	fclose(handle);

	 printf("Current PID: %d\n", thePid);
	while(num_jobs-- > 0)
	{
		/* YIELD */
		struct timeval t1, t2;
		int wakeup_t, job_t;
		gettimeofday(&t1,NULL);
		wakeup_t = (t1.tv_sec - t0.tv_sec)*1000000 + t1.tv_usec - t0.tv_usec;
		printf("Wakeuptime = %d us\n", wakeup_t);	
		fib(n);
		handle = fopen(proc_filename ,"r+");
		fprintf(handle, "Y, %d", thePid);
		fclose(handle);

		gettimeofday(&t2,NULL);
		job_t = (t2.tv_sec - t1.tv_sec)*1000000 + t2.tv_usec - t1.tv_usec;
		printf("Job Process time = %d us\n", job_t);	
	}

	handle = fopen(proc_filename, "r+");
	fprintf(handle, "D, %d", thePid);
	fclose(handle);

	return 0;
}
