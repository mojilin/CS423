#ifndef HWM_H
#define HWM_H

#include <pthread.h>

double get_cpu_use();
void set_throttle(double new_throttle);
double get_throttle();

#endif