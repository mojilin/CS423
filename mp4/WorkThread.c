#include <unistd.h>
#include "WorkThread.h"

void worker_thread(void *arg) {
	arg_t * args = (arg_t *)arg;

	alarm(1 - args->throttle); // 1 - throttle seconds
}