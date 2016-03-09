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
	int n = 45;
	char tempstring[100]; /* temp string */
	int i = 0;
	char temp[5];
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
	while((tempstring[i] = fgetc(handle)) != EOF)
	{
		i++;
		if(tempstring[i - 1] == '\n')
		{
			tempstring[i - 1] = '\0';
			i = 0;
			sscanf(tempstring, "%s %d", temp, &tempPID);
			if(tempPID == thePid)
			{
				printf("Registration successful!\n");
				break;
			}
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
	printf("Fib:%d = %lld\n", n, fib(n));

	handle = fopen(proc_filename, "r+");
	fprintf(handle, "D, %d", thePid);
	fclose(handle);

	return 0;
}
