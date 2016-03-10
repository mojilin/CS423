#include "userapp.h"
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const char proc_filename[] =  "/proc/mp2/status";

long long fib(int n)
{
	if(n ==0)
		return 0;
	if( n == 1)
		return 1;

	return fib(n-1) + fib(n-2);
}

int factorial(int n)
{
	if(n == 0)
		return 1;
	return n * factorial(n-1);
}


int main(int argc, char* argv[])
{
	FILE* handle;
	int thePid = (int) getpid();
	int period = 100;	/* Period of the job in milliseconds*/
	int computation = 10; /* Processing time of job in milliseconds*/
	int n = 20;
	char tempstring[100]; /* temp string */
	int i = 0;
	int num_jobs = 100;
	int tempPID;


	if(handle == NULL)
	{
		fprintf(stderr, "Could not open file\n");
		return -1;
	}
	handle = fopen(proc_filename ,"r+");
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
			fscanf(handle, "%[^\n]%*c", tempstring);
			sscanf(tempstring, "%*s %d", &tempPID);
			if(tempPID == thePid)
			{
				printf("Registration successful!\n");
				break;
			}
		if(feof(handle))
		{
			printf("FILE registration failed\n");
			return -1;
		}
	}
	fclose(handle);

	/* YIELD */
	handle = fopen(proc_filename ,"r+");
	fprintf(handle, "Y, %d", thePid);
	fclose(handle);

	printf("Current PID: %d\n", thePid);
	while(num_jobs > 0)
	{
		printf("Job%d = %lld\n",num_jobs-- , fib(n));
		/* YIELD */
		handle = fopen(proc_filename ,"r+");
		fprintf(handle, "Y, %d", thePid);
		fclose(handle);
		
	}

	handle = fopen(proc_filename, "r+");
	fprintf(handle, "D, %d", thePid);
	fclose(handle);

	return 0;
}
