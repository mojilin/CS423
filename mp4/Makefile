
FLAGS:= -Wall -g -lpthread
GCC:=gcc
RM:=rm

.PHONY : clean

all: clean server client

server:
	gcc networking.c comms.c ${FLAGS} -o server

client:
	gcc local_node.c networking.c ${FLAGS} -o client


clean:
	$(RM) -f server client *~ *.o


