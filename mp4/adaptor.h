#ifndef ADAPTOR_H
#define ADAPTOR_H

#include "HWMonitor.h"

typedef struct {
	node_state * remote_state;
	node_state * local_state;
} adaptor_arg_t;

void * adaptor_thread(void * arg);

#endif