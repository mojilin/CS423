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
	char str[15];
	fprintf(handle, "%d", thePid);

	//memset(str, 0, 15);
	printf("Current PID: %d\n", thePid);

	//printf("Read String:\n %s\n", str);

	return 0;
}
