PROGS=server
CC=gcc
CFLAGS=-w -g  


all: server cleanobjects 

server:
	$(CC) -pthread -o server $(CFLAGS) india_messages_protocol.c india_messages_server.c
cleanobjects:
	rm -f *.o

clean:
	rm -f ${PROGS} *~ *.a
