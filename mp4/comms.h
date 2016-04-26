
#ifndef COMMS_H
#define COMMS_H


#include "networking.h"
#include "HWMonitor.h"
#include "job.h"

void transfer_job(int fd, Job_t job);

void send_state(int fd, node_state state);




#endif

