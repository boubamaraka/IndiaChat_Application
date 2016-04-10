PROGS=client
CC=gcc
SQLFLAGS=-Wall
CFLAGS=-w -g


all: client cleanobjects

client:
	$(CC) -o client $(CFLAGS) india_message_register.c india_messages_protocol.c -pthread
cleanobjects:
	rm -f *.o

clean:
	rm -f ${PROGS} *~ *.a
