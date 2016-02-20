#include "userapp.h"
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	FILE* handle = fopen("/proc/mp1/status","r+");
	int thePid = (int) getpid();
	fprintf(handle, "%d", thePid);

	printf("Current PID: %d\n", thePid);
	
	fclose(handle);

	return 0;
}
