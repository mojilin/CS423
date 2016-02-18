#include "userapp.h"
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	int handle = open("/proc/mp1/status");
	int thePid = (int) getpid();
	char str[15];
	int n = sprintf(str, "%d", thePid);
	write(handle, str, n);


	return 0;
}
