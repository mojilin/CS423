#include "userapp.h"
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

const char * proc_filename =  "/proc/mp2/status";

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
	FILE* handle = fopen(proc_filename ,"r+");
	int thePid = (int) getpid();
	int period = 100;	/* Period of the job in milliseconds*/
	int computation = 10; /* Processing time of job in milliseconds*/
	int n = 50;
	/* Registration */
	fprintf(handle, "R, %d, %d, %d", thePid, period, computation);
	// fclose(handle);

	printf("Current PID: %d\n", thePid);
	
	// printf("Fib:%d = %lld\n", n, fib(n));

	// handle = fopen(proc_filename, "r+");
	fprintf(handle, "D, %d", thePid);


	return 0;
}
