#ifndef INDIA_MESSAGES_PROTOCOL
#define INDIA_MESSAGES_PROTOCOL
/*****
GENERAL INCLUDE FILES
*****/
#include <sys/socket.h>  // defines socket, connect, ...
#include <netinet/in.h>  // defines sockaddr_in
#include <string.h>      // defines memset
#include <stdio.h>       // defines printf, perror, ...
#include <arpa/inet.h>   // inet_pton, ...
#include <unistd.h>      // read, ...
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>
#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
//color codes
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

/*
DEFINITIONS FOR INDIA MESSAGES
*/
#define BUFFER_LENGHT 100
#define NAME_LENGHT 				10
#define CHAT_ROOM_NAME_LENGHT 		20
#define	PW_LENGHT					5

// error codes from server:
#define SUCCESS						0
#define NICK_NAME_IN_USE				1 // possible error code for register
#define	CHAT_ROOM_DOES_NOT_EXIST 	2 // possible error code for join- or exit- or data- response message
#define	CHAT_ROOM_NAME_IN_USE		3 // possible error code for create
#define USER_DOES_NOT_EXIST			4 // possible error code for data- response message
#define WRONG_PW 					5 // one error code for login

//	message IDs from client to server
#define REGISTER_USER			10
#define LOGIN 					20
#define	SHOW_CHAT_ROOMS	30
#define SHOW_USERS 			40
#define JOIN			 		50
#define EXIT			 		70
#define CREATE_CHAT_ROOM	80

//	message ID for both directions
#define CHAT_ROOM_DATA 		60
#define	PRIVATE_DATA			90
#define EXIT_FROM_SERVICE	100
// message ID for data ack from server to client

//	message IDs from server to client
#define REGISTER_USER_RESPONSE 			11
#define LOGIN_RESPONSE 					21
#define	SHOW_CHAT_ROOMS_RESPONSE		31
#define SHOW_USERS_RESPONSE 			41
#define JOIN_RESPONSE 					51
#define	CHAT_DATA_RESPONSE				61
#define PRIVATE_DATA_RESPONSE			91
#define EXIT_RESPONSE 					71
#define CREATE_CHAT_ROOM_RESPONSE 	81
#define EXIT_FROM_SERVICE_RESPONSE		101
//	multipurpose message for sending and receiving


typedef struct india_universal_message
{
	int 	id;
	char 	sender[NAME_LENGHT];   				// client nickname who's sending
	char	receiver[NAME_LENGHT] ; 				// receiver: nickname or chat_room
	char 	proposed_nickname[NAME_LENGHT]	;
	char 	password[PW_LENGHT];
	char	chat_room_name[CHAT_ROOM_NAME_LENGHT];  		// used to create, join and to exit from chat room
	int		number_of_users; 						// how many users are in "users" list in msg 41
	int		number_of_chat_rooms; 					// how many users are in "users" list in msg 41
	int		error_code;
	int 	data_lenght;
	int socket;
	char data[1024];
} india_universal_message;


//	declare the sending function

int send_universal_india_message(struct india_universal_message msg, int sockfd, char* ptr);

//	declare the reading function

int read_universal_india_message(struct india_universal_message* msg, int sockfd, char **ptr);

//	declare the udp sending function	

int sendto_universal_india_message( struct india_universal_message msg, int sockfd, char* ptr, struct sockaddr_in in_address);

//	declare the udp reading function

int recvfrom_universal_india_message( struct india_universal_message* msg,  int sockfd, char **ptr, struct sockaddr_in *in_address);


//User getting register , username from the terminal
void username_check(char *username,char *password);
//function to let a user type a character without being server
int getch(void);
//Register user and send username to the server
void register_user(india_universal_message msg);
//Loginuser and send username to the server
void Login(india_universal_message msg);
// setting up connecting to the server
void connection_to_server(india_universal_message msg,char *Ip_addr,char *port_num);
//Find out if the user is register or not .
char user_status();
//the rules of the applications
void Chat_Rule();
//handle  user input
void control_user_input(india_universal_message connection);
//handle the reading from the server side
void handle_server_message(india_universal_message * connection);
//handle login to server
int handle_login(india_universal_message msg);
//handle user registration
int handle_registration(india_universal_message msg);
//login or register handling
void register_or_login(india_universal_message msg);
// Removes the  newline character from a string.
void remove_newline_caracter(char *str);
//prints the time info for the chat
void india_message_chat_time(char str [20]);

#endif
