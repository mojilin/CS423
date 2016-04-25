#include <stdlib.h>
#include <stdio.h>
#include "HWMonitor.h"

pthread_spinlock_t throttle_lock;
double throttle = 0;

void throttle_thread();

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

void set_throttle(double new_throttle) {
	pthread_spin_lock(&throttle_lock);
	throttle = new_throttle;
	pthread_spin_unlock(&throttle_lock);
}

double get_throttle() {
	return throttle;
}

void throttle_thread() {
	double throttle_in = 0.0;
	while (SOME CONDITION) {
		printf("Set throttle pls: ");
		scanf("%f", &throttle_in);

		if (throttle_in >= 0.0 && throttle_in <= 1.0) {
			set_throttle(throttle_in);
			printf("Throttle set to %f.\n", throttle_in);
		} else {
			printf("That ain't right.\n");
		}

		throttle_in = 0.0;
	}
}