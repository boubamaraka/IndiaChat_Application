PROGS=client
CC=gcc
CFLAGS=-w -g  
SQLFLAGS=-Wall


all: client cleanobjects

client:
	$(CC) -o client $(CFLAGS) india_message_register.c india_messages_protocol_udp.c -pthread
cleanobjects:
	rm -f *.o

clean:
	rm -f ${PROGS} *~ *.a
