
#include "client.h"
//function getch caracter
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}
//User registration

void username_check(char *username,char *password){
    int size=0;
    //memset(username, 0, 1000);
    while(1){
      printf("Enter your username :");
      fflush(stdout);
      memset(username, 0, 1000);
      fgets(username,30, stdin);
      size=(int)strlen(username);
      printf("size %d",size);
      if(size>20){

        printf("Your user name is too long It should be less than 20 but you have %d\n",size);

     }
     else if(size<=20){
       char str[15],c=' ';int i=0;
       printf("your username is :\n%s",username);
       printf("Enter your password <Max 10 carater>:\n");

        while (i<=9){
          str[i]=getch();
          c=str[i];
          if(c=='\n') {
              break;
          }
          else{ printf("*");
              }
          i++;
              }
              str[i]='\0';
       strcpy(password,str);
       printf("password is :%s\n",password);


       //fgets(password,50, stdin);
       break;
     }
     size=0;

 }
}
//check the status of the user
void user_status(char *resp){
    //char resp[2];
  //memset(resp,0,strlen(resp));
  printf("Do you want to Register or Login? press L/l for login or R/r for Registration:\n");
  fgets(resp,2, stdin);

  while(1){

       if(strcmp(resp,"l")==0||strcmp(resp,"L")==0){
         printf("you have to login\n");
         break;
       }
       else if (strcmp(resp,"r")==0||strcmp(resp,"R")==0){
         printf("you have to register");
         break;
       }
       printf("Please press L/l for login or R/r for Registration: \n");
        memset(resp,0,sizeof(resp));
        fgets(resp,2, stdin);
  }


}

//send user information for registration to the server
void register_user(Client_info *con_client){
  message msg;
  msg.msg_type =REGISTER_USER;
  strncpy(msg.user, con_client->user, 21);
  strncpy(msg.password, con_client->password, 21);
  printf("name:%s\n",con_client->user);
  printf("password:%s\n",con_client->password);
  if(send(con_client->socket, (void*)&msg, sizeof(msg), 0) < 0)
  {
    perror("Failed to register to server");
    exit(1);
  }

}
//send user information to be  login
void Login(Client_info *con_client){
  message msg;
  msg.msg_type =LOGIN_USER;
  strncpy(msg.user, con_client->user, 21);
  strncpy(msg.password, con_client->password, 21);

  if(send(con_client->socket, (void*)&msg, sizeof(msg), 0) < 0)
  {
    perror("Failed to login to server");
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
  struct addrinfo hints, *res,*ressave;
  int value;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM;
  char resp[2];

  if ( (value= getaddrinfo(Ip_addr,port_num, &hints, &res)) != 0) {
         fprintf(stderr, "Connection error for  %s\n", gai_strerror(value));
         exit(1);
 }
 ressave = res;
 do {
         con_client->socket = socket(res->ai_family, res->ai_socktype,
                          res->ai_protocol);
          if (con_client->socket < 0)
                  continue;
          printf("The address version is %s \n", res->ai_family == AF_INET ? "IPv4" : "IPv6");


          if (connect(con_client->socket, res->ai_addr, res->ai_addrlen) == 0)
                break;
          printf("connect failed\n");

          close(con_client->socket);
  } while ( (res = res->ai_next) != NULL);
  free(ressave);
  Chat_Rule();
  control_user_input(con_client);
  strcpy(resp,"R");

 //user_status(resp);
  while(1){


  printf("%s\n",resp);
   username_check(con_client->user,con_client->password);
   printf("the struct username : %s\n",con_client->user);
   printf("the struct password: %s\n",con_client->password);

  message newmsg;
  ssize_t receive;
  if(strcmp(resp,"R")==0||strcmp(resp,"r")==0){
  register_user(con_client);
  receive = recv(con_client->socket, &newmsg, sizeof(message), 0);
  if(receive < 0)
  {
      perror("failed to receive");
      exit(1);

  }
  else if(receive == 1)
  {
    close(con_client->socket);
    printf("The username \"%s\" exist already , please try another.\n", con_client->user);
    continue;
  }
}
else{
  //login to the server
    Login(con_client);
   receive = recv(con_client->socket, &newmsg, sizeof(message), 0);
  if(receive < 0)
  {
      perror("failed to receive");
      exit(1);

  }
  else if(receive == 0)
  {
    close(con_client->socket);
    printf("Login failed, You type wrong login Try Again");
    continue;
  }

}

  break;
}

  printf("%s Connected to server.",con_client->user);
  printf("Type /help for usage.");
}
void Chat_Rule(){

	printf("###########################################################\n");
	printf("#          Those are the rules to follow:                 #\n");
  printf("###########################################################\n");
	printf("To exit type /help\n");
  printf("To exit type /exit\n");
	printf("To quit type /quit\n");
  printf("To show chat room type /chatrooms or /list\n");
  printf("To show users type /users or /showusers\n");
  printf("/PVM <username> Send a private message to given username\n");
  printf("/Group <Groupname> Send a public message to given groupname\n");

}

void control_user_input(Client_info *connection){
  char input[100];
  //Chat_Rule();
  fgets(input, 100, stdin);
  printf("input enter:%s\n",input);
  if(strcmp(input,"/help\n") == 0 ||strcmp(input,"/h\n")==0)
  {
    printf("helping you now\n");
  //  Chat_Rule();
  }
  if(strcmp(input,"/q\n") == 0 || strcmp(input,"/quit\n") == 0)
  {
    printf("connection is being closed\n");
    close_socket(connection);
  }
  else if(strcmp(input,"/chatrooms\n") == 0 || strcmp(input,"/list\n") == 0){
    message msg;
    msg.msg_type=SHOW_CHAT_ROOMS;
    printf("list");
    if(send(connection->socket, &msg, sizeof(message), 0) < 0)
    {
        perror("Send failed\n");
        exit(1);
    }

  }
  else if(strcmp(input,"/users\n") == 0 || strcmp(input,"/showusers\n") == 0){
    message msg;
    msg.msg_type=SHOW_USERS;
    printf("users");
    if(send(connection->socket, &msg, sizeof(message), 0) < 0)
    {
        perror("Send failed");
        exit(1);
    }

  }
  else if(strncmp(input,"/PVM",4) == 0){
    message msg;
    char *destination;
    char data[1024];
    //get whatever comes after /PVM
    destination =strtok(input+5," ");
    printf("PVM");
    printf(" destination name:%s\n",destination);
    if( destination == NULL)
  {
    puts(KRED " You enter a NULL , The format for private messages is: /MPV <username> " RESET);
    return;
  }

  if(strlen(destination) == 0)
  {
    puts(KRED "To chat In private you must give a Username." RESET);
    return;
  }

  if(strlen(destination) > 20)
  {
    puts(KRED "The username must be between 1 and 20 characters." RESET);
    return;
  }
  printf(">>YOU:");
  fgets(data,sizeof(data),stdin);

    msg.msg_type=PRIVATE_MESSAGE;
    strcpy(msg.user,destination);
    strcpy(msg.buffer,data);
    printf("user:%s",msg.user);
    printf("buffer:%s",msg.buffer);

    if(send(connection->socket, &msg, sizeof(message), 0) < 0)
    {
        perror("message sending failed");
        exit(1);
    }

  }
  //Joing a chatroom and sending public message
  else if(strncmp(input,"/Group",6) == 0){
    message msg;
    char *group;
    char data[1024];
    //get whatever comes after /Group
    group =strtok(input+7," ");
    printf("group name:%s\n",group);
    if( group == NULL)
  {
    puts(KRED " You enter a NULL , The format for Joning group : /Group <groupname> " RESET);
    return;
  }

  if(strlen(group) == 0)
  {
    puts(KRED "To join chat group you must give a name you would like to join." RESET);
    return ;
  }

  if(strlen(group) > 20)
  {
    puts(KRED "The groupname must be between 1 and 20 characters." RESET);
    return;
  }
    msg.msg_type=GROUP;
    printf("grouping stuff");
    strncpy(msg.user, connection->user, 21);
    if(send(connection->socket, &msg, sizeof(message), 0) < 0)
    {
        perror("Send failed");
        exit(1);
    }

  }

}
//read server message
void read_server_message(Client_info *connection){
  message msg;
  //Receive a reply from the server
  ssize_t receive = recv(connection->socket, &msg, sizeof(message), 0);
  if(receive < 0)
  {
      perror("recv failed");
      exit(1);

  }
  else if(receive == 0)
  {
    close(connection->socket);
    puts("Server disconnected.");
    exit(0);
  }
}

int main(int argc, char *argv[]){
  //check the input in the terminal
  Client_info connection;
  if (argc != 3) {
        printf("Usage: %s [server] [port]\n", argv[0]);
        return 0;
    }
 connection_to_server(&connection, argv[1], argv[2]);



  return 1;


}
