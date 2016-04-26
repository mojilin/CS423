
FLAGS:= -Wall -g -lpthread
GCC:=gcc
RM:=rm

.PHONY : clean

all: clean job server client network 


job_queue:
	gcc job_queue.c ${FLAGS} -c -o job_queue.o

job: job_queue
	gcc job.c  ${FLAGS} -c -o job.o

network:
	gcc networking.c ${FLAGS} -c -o networking.o

worker: job_queue
	gcc WorkThread.c ${FLAGS} -c -o  WorkThread.o

HW_Monitor:
	gcc HWMonitor.c ${FLAGS} -c -o HWMonitor.o

comms: network
	gcc comms.c ${FLAGS} -c -o comms.o

client: network job worker comms HW_Monitor
	gcc -D LOCAL local_node.c networking.o comms.o HWMonitor.o WorkThread.o job_queue.o job.o ${FLAGS} -o client

server: client
	gcc -D REMOTE local_node.c networking.o comms.o HWMonitor.o WorkThread.o job_queue.o job.o ${FLAGS} -o server

clean:
	$(RM) -f server client *~ *.o


