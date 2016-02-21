#include "userapp.h"
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

long long fib(int n)
{
	if(n ==0)
		return 0;
	if( n == 1)
		return 1;

	return fib(n-1) + fib(n-2);
}


int main(int argc, char* argv[])
{
	FILE* handle = fopen("/proc/mp1/status","r+");
	int thePid = (int) getpid();
	int n = 50;
	fprintf(handle, "%d", thePid);
	fclose(handle);
	printf("Current PID: %d\n", thePid);
	
	printf("Fib:%d = %lld\n", n, fib(n));

	return 0;
}
