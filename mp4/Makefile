
FLAGS:= -Wall -g -lpthread
GCC:=gcc
RM:=rm

.PHONY : clean

all: clean server client network job


job_queue:
	gcc job_queue.c ${FLAGS} -c -o job_queue.o

job: job_queue
	gcc job.c ${FLAGS} -c -o job.o

network:
	gcc networking.c ${FLAGS} -c -o networking.o

server: network
	gcc networking.o comms.c ${FLAGS} -o server

client: network
	gcc local_node.c networking.c ${FLAGS} -o client


clean:
	$(RM) -f server client *~ *.o


