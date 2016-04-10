PROGS=server client
CC=gcc
CFLAGS=-Wall -g -I./protocol -I./server -I./client
 

all: server client cleanobjects

server:
	$(CC) -o server $(CFLAGS) india_messages_protocol.c india_messages_server.c
client:
	$(CC) -o client $(CFLAGS) india_messages_protocol.c india_messages_test_client.c
cleanobjects:
	rm -f *.o

clean:
	rm -f ${PROGS} *~ *.a
