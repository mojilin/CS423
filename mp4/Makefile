
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

server: network
	gcc networking.o comms.c ${FLAGS} -o server

client: network job
	gcc local_node.c networking.c  job_queue.o job.o ${FLAGS} -o client


clean:
	$(RM) -f server client *~ *.o


