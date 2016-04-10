#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdbool.h>
//#include <curses.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
typedef struct {
    pthread_t thread_id;
    int socketfd;
} serv_thread;


//#include <pthread.h>
//color codes
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

#define MAXIMUM 50
#define BUFF_SIZE 1024
typedef enum
{
  CONNECTED,
  DISCONNECTED,
  GET_USERS,
  REGISTER_USER,
  LOGIN_USER,
  SHOW_CHAT_ROOMS,
  SHOW_USERS,
  GROUP,
  PUBLIC_MESSAGE,
  PRIVATE_MESSAGE,
  TOO_FULL,
  USERNAME_ERROR,
  SUCCESS,
  ERROR

} Type_Msg;
//message structure
typedef struct
{ Type_Msg msg_type;
  char user[21];
  char password[21];
  char buffer[BUFF_SIZE];

} message;

//structure to hold client con_cliention information
typedef struct Client_info
{
  int socket;
  struct sockaddr_in Client_addr;
  char user[21];
  char password[21];
} Client_info;
//function to let a user type a character without being server
int getch(void);
//check the length of the username the maximum length that username can have is 20
void check_user(char *username);
//User getting register , username from the terminal
void username_check(char *username,char *password);
//Register user and send username to the server
void register_user(Client_info *con_client);
// close the socket
void close_socket(Client_info *con_client);
// setting up connecting to the server
void connection_to_server(Client_info *con_client,char *Ip_addr,char *port_num);
//client username
int client_username(Client_info *con_client);
//Find out if the user is register or not .
void user_status(char * resp);
//user Login
void Login(Client_info *con_client);
//handle user input
void control_user_input(Client_info *connection);
//the rules of the applications
void Chat_Rule();
//user in the chatroom
void Join_chatroom(char *chatroom);
//read server message for control user input
void read_server_message(Client_info *connection);
