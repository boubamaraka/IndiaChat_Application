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
//#include <pthread.h>
#define MAXIMUM 50
#define BUFF_SIZE 1024
typedef enum
{
  con_client,
  DIScon_client,
  GET_USERS,
  SET_USERNAME,
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
  char buffer[BUFF_SIZE];

} message;

//structure to hold client con_cliention information
typedef struct Client_info
{
  int socket;
  struct sockaddr_in Client_addr;
  char user[20];
} Client_info;

//check the length of the username
void check_username(char *username){
    char * user;
    user=username;
    while(1){
      if(strlen(user)>20){
        printf("Your user name is too long It should be less than 20 but you have %d\n",(int)strlen(user));
        printf("Enter your username again :");
        //memset(user,0,sizeof(user));
        fgets(user,50, stdin);
        //check_username(user);
     }
     else{
       printf("your username is :%s",user);
       break;
     }

 }
}

//get username from the terminal
void register_user(){
  char user[MAXIMUM];
  char userpass[MAXIMUM];
    printf("Enter your Username:");
    fgets(user,sizeof(user), stdin);
    check_username(user);

    printf("Enter your Password:");
    fgets(userpass,sizeof(userpass), stdin);



}
//send user nickname to the server
void send_username(Client_info *con_client){
  message msg;
  msg.msg_type = SET_USERNAME;
  strncpy(msg.user, con_client->user, 20);
  if(send(con_client->socket, (void*)&msg, sizeof(msg), 0) < 0)
  {
    perror("Failed to send username to server");
    exit(1);
  }

}
// close the socket
void close_socket(Client_info *con_client){
  close(con_client->socket);
  exit(1);
}
// setting up connecting to the server
void connection_to_server(Client_info *con_client,char *Ip_addr,char *port_num){
  while(1){
  //socket is created
  if ((con_client->socket = socket(AF_INET, SOCK_STREAM , IPPROTO_TCP)) < 0)
  {
      perror("Failed to creat socket");
  }
  con_client->Client_addr.sin_family=AF_INET;
  con_client->Client_addr.sin_port=htons(atoi(port_num));
  con_client->Client_addr.sin_addr.s_addr = inet_addr(Ip_addr);
  //connect to the server
  if (connect(con_client->socket, (struct sockaddr *)&con_client->Client_addr , sizeof(con_client->Client_addr)) < 0)
  {
      perror("connection to the server failed.");
      exit(1);
  }
  send_username(con_client);
  message newmsg;
  ssize_t receive;
  receive = recv(con_client->socket, &newmsg, sizeof(message), 0);
  if(receive < 0)
  {
      perror("failed to receive");
      exit(1);

  }
  else if(receive == 0)
  {
    close(con_client->socket);
    printf("The username \"%s\" exist already , please try another.\n", con_client->user);
    continue;
  }

  break;
}

}
