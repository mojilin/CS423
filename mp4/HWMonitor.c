#include <stdlib.h>
#include <stdio.h>
#include "HWMonitor.h"

double get_cpu_use() {
	double a[4], b[4], load;
	FILE *fp;

	fp = fopen("/proc/stat","r");
	fscanf(fp,"%*s %f %f %f %f",&a[0],&a[1],&a[2],&a[3]);
	fclose(fp);
	sleep(1);

	fp = fopen("/proc/stat","r");
	fscanf(fp,"%*s %f %f %f %f",&b[0],&b[1],&b[2],&b[3]);
	fclose(fp);

	loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2])) / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));

	return loadavg;
}