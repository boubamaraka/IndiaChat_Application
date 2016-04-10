
#include "india_messages_protocol.h"

typedef struct {
    pthread_t thread_id;
    int socketfd;
} client_thread;

void *input_reader(void *arg) {
  client_thread *client_id;
  int sockfd;
	ssize_t n;
  char buffer[1024];
  india_universal_message *client;


    client_id = (client_thread*) arg;
    sockfd = client->socket;

    printf("input_reader socket: %d\n", client->socket);
    // Read socket in a loop
    for (;;) {
       // n = read(sockfd, buffer, 1023);
        memset(buffer,0,sizeof(buffer));

        if ((n = read(sockfd,buffer,&client)) > 0){
            puts(buffer);
        }else{
            //perror("read error");
            break;
        }
    }

    pthread_exit(arg);
}



char join_creat_chatroom(){
   char resp;
  printf("Do you want to Creat or Join? press C/c for creat chatroom or J/j for Joining chatroom \n");
  scanf("  %c", &resp );

  while(1){

       if(resp=='c'||resp=='C'){
         printf("-----Creating Chatroom-------\n");
         break;
       }
       else if (resp=='J'||resp=='j'){
         printf("Joining Chat room\n");
         break;
       }
       printf("Please press  C/c for creat chatroom or J/j for Joining chatroom: \n");

       scanf("  %c", &resp );
  }
return resp;

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
//check the status of the user
char user_status(){
   char resp;
  printf("Do you want to Register or Login? press L/l for login or R/r for Registration \n");
  scanf("  %c", &resp );

  while(1){

       if(resp=='l'||resp=='L'){
         printf("you have to login\n");
         break;
       }
       else if (resp=='r'||resp=='R'){
         printf("you have to register\n");
         break;
       }
       printf("Please press L/l for login or R/r for Registration: \n");

       scanf("  %c", &resp );
  }
return resp;

}
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

void username_check(char *username,char *password)
{

    while(1){
      //printf("Enter your username :");
      fflush(stdout);
      memset(username,0,sizeof(username));
      fgets(username,20, stdin);
      //strcpy(username,user);
      if((int)strlen(username)>10||(int)strlen(username)==1){

        printf("Please:Enter username that is  between 1  to 10 character :\n");

     }else {
       char str[15],c=' ';int i=0;
        printf("username lenght %d:", (int)strlen(username));
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
       printf("\npassword is :%s\n",password);


       //fgets(password,50, stdin);
       break;
     }


 }
}


//send user information for registration to the server
void register_user(india_universal_message msg){

  msg.id =REGISTER_USER;
  char *buffer;
  printf("register name:%s\n",msg.proposed_nickname);
  printf("register password:%s\n",msg.password);
  if(send_universal_india_message(msg, msg.socket,buffer) < 0)
  {
    perror("Failed to register to server");
    exit(1);
      printf("Debug = %d\n",__LINE__);
  }
  printf("Debug = %d\n",__LINE__);
}
//send user information to be  login
void Login(india_universal_message msg){
  char *buffer;
  msg.id =LOGIN;
  printf("Login name:%s\n",msg.proposed_nickname);
  printf("Login password:%s\n",msg.password);
	strcpy(msg.sender,msg.proposed_nickname);
  printf("sender name:%s\n",msg.sender);

  if(send_universal_india_message(msg, msg.socket,buffer) < 0)
  {
    perror("Failed to Login to server");
    exit(1);
  }


}

// close the socket
void close_socket(india_universal_message msg){
  close(msg.socket);
  exit(1);
}
//user input controler
void control_user_input(india_universal_message connection){
  char input[100];
  char *buffer;int val1;
  india_universal_message newmsg;
  CHATRULE:
  Chat_Rule();
  fgets(input, 100, stdin);
  printf("input enter:%s\n",input);
  if(strcmp(input,"/help\n") == 0 ||strcmp(input,"/h\n")==0)
  {
    printf("-------------------helping you now------------------\n");
    Chat_Rule();
  }
  else if(strcmp(input,"/q\n") == 0 || strcmp(input,"/quit\n") == 0)
  {
    printf("-------------------connection is being closed-----------------\n");
    close_socket(connection);
  }
  else if(strcmp(input,"/chatrooms\n") == 0 || strcmp(input,"/list\n") == 0){
    india_universal_message msg;
    msg.id=SHOW_CHAT_ROOMS;
    printf("--------------List-----------------------------\n");
    if(send_universal_india_message(msg, connection.socket,buffer) < 0)
    {
      perror("Failed sending to server");
      exit(1);
    }

  }
  else if(strcmp(input,"/users\n") == 0 || strcmp(input,"/showusers\n") == 0){
   india_universal_message msg;
   char *buffer2;
    msg.id=SHOW_USERS;
    printf("----------------------Users-------------------------\n");
    if(send_universal_india_message(msg, connection.socket,buffer) < 0)
    {
      perror("Failed sending to server");
      exit(1);
    }

  }
  else if(strncmp(input,"/PVM",4) == 0){

    char *destination;
    char data[100];
    india_universal_message msg ,newmsg;
    //get whatever comes after /PVM
    destination =strtok(input+5," ");
    printf("PVM");
    printf(" destination name:%s\n",destination);
    if( destination == NULL)
  {
    puts(KRED " You enter a NULL , The format for private messages is: /MPV <username> " RESET);
    goto CHATRULE;
  }

  if(strlen(destination) == 0)
  {
    puts(KRED "To chat In private you must give a Username." RESET);
    goto CHATRULE;
  }

  if(strlen(destination) > 20)
  {
    puts(KRED "The username must be between 1 and 20 characters." RESET);
    goto CHATRULE;
  }
  printf(">>YOU:");
  fgets(data,100,stdin);
    //india_universal_message msg;
    msg.id=PRIVATE_DATA;
    strcpy(msg.receiver,destination);
    strcpy(msg.sender,connection.proposed_nickname);
		printf("proposename:%s\n",connection.proposed_nickname);
    msg.data_lenght=(int)strlen(data);


    printf("receiver username:%s\n",msg.receiver);
    printf("sender username:%s\n",msg.sender);
    printf("length of the messages:%d\n",msg.data_lenght);
    char *buffer2;

      int val;
      val=send_universal_india_message(msg,connection.socket,data);
       printf("send value of val:%d",val);
      if(val < 0)
      {
        perror("Failed sending to server");
        exit(1);
      }


  }
  //Joing a chatroom and sending public message
  else if(strncmp(input,"/Group",6) == 0){

    char *group;
    char *data;
    //get whatever comes after /Group
    group =strtok(input+7," ");
    printf("group name:%s\n",group);
    if( group == NULL)
  {
    puts(KRED " You enter a NULL , The format for Joning group : /Group <groupname> " RESET);
    goto CHATRULE;
  }

  if(strlen(group) == 0)
  {
    puts(KRED "To join chat group you must give a name you would like to join." RESET);
     goto CHATRULE;
  }

  if(strlen(group) > 20)
  {
    puts(KRED "The groupname must be between 1 and 20 characters." RESET);
    goto CHATRULE;
  }
    india_universal_message msg,newmsg;
    char *buffer2;
		char resp;
		resp=join_creat_chatroom();
		if (resp=='c'||resp=='C'){

    msg.id=CREATE_CHAT_ROOM;
	}
	else {

	msg.id=JOIN;
}
    strcpy(msg.chat_room_name,group);
    strcpy(msg.sender,connection.proposed_nickname);
    printf("----------Joining group--------------------\n");
    printf("chatroom name:%s\n",msg.chat_room_name);
    printf("joining name:%s\n",msg.sender);
  //  strncpy(msg.user, connection->user, 21);
  int val,val1;
  val =send_universal_india_message(msg, connection.socket,buffer);
  printf("value %d:\n",val);
  if( val< 0)
  {
    perror("Failed sending to server");
    exit(1);
  }


  }
  else{
    puts(KRED "you are out of option" RESET);

    goto CHATRULE;
  }

}
void handle_server_message(india_universal_message connection)
{
  india_universal_message newmsg={0},msg;
  char *buffer2;
   int val1=read_universal_india_message(&newmsg,connection.socket,&buffer2);

  if(val1 < 0)
  {
    //perror("Failed reading from server");
  //  exit(1);
  }
  else if(val1 == 0)
  {
    close(connection.socket);
    puts("Server disconnected.");
    exit(0);
  }

  switch(msg.id)
  {

    case SHOW_USERS:
      printf("%s", newmsg.proposed_nickname);
    break;

    case PRIVATE_DATA:
      printf(KGRN "%s" RESET ": %s\n", newmsg.proposed_nickname, newmsg.data);
    break;

    case CHAT_ROOM_DATA:
      printf(KWHT "From %s:" KCYN " %s\n" RESET, newmsg.proposed_nickname, newmsg.data);
    break;

    default:
      //fprintf(stderr, KRED "Unknown message type received." RESET "\n");
    break;
  }
}


// setting up connecting to the server
void connection_to_server(india_universal_message msg,char *Ip_addr,char *port_num){
  struct addrinfo hints, *res,*ressave;
  int value;


  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM;

  if ( (value= getaddrinfo(Ip_addr,port_num, &hints, &res)) != 0) {
         fprintf(stderr, "Connection error for  %s\n", gai_strerror(value));
         exit(1);
 }
 ressave = res;
 do {
         msg.socket = socket(res->ai_family, res->ai_socktype,
                          res->ai_protocol);
          if (msg.socket < 0)
                  continue;
          printf("The address version is %s \n", res->ai_family == AF_INET ? "IPv4" : "IPv6");


          if (connect(msg.socket, res->ai_addr, res->ai_addrlen) == 0)
                break;
          printf("connect failed\n");
// return failure if connection fails
          close(msg.socket);
  } while ( (res = res->ai_next) != NULL);
  free(ressave);
  fd_set file_descriptors;
   register_or_login(msg);


}
void register_or_login(india_universal_message msg)
{
	client_thread *client;
	void *thread_join;
	int n;
  int retval = 0;
  while(retval == 0){

      char status=user_status();
      username_check(msg.proposed_nickname,msg.password);
      printf("the struct username : %s\n",msg.proposed_nickname);
      printf("the struct password: %s\n",msg.password);

      if (status=='r'|| status=='R'){
        retval = handle_registration(msg);
      }
      else{
        retval = handle_login(msg);
      }
    }

		control_user_input(msg);
		n = pthread_create(&(client->thread_id), NULL, input_reader, (void*)client);
		printf("input_reader thread created\n");
		//n = pthread_join(client->thread_id, &thread_join);
}


int handle_registration(india_universal_message msg)
{
    int receive=0;
    india_universal_message newmsg = {0};
    char *buffer;
    int retval = 0;

    printf("-------------Register Process----------------------\n");
    register_user(msg);
    receive = read_universal_india_message(&newmsg,msg.socket,&buffer);
    printf("message ID :%d\n",newmsg.id);
    printf("error code :%d\n",newmsg.error_code);
    printf("receive value:%d\n",receive);

    if(receive < 0)
    {
      perror("failed to receive");
    }
    else if (newmsg.error_code == 0)
    {
      retval = 1;
    }
    else if(newmsg.error_code == NICK_NAME_IN_USE)
    {

      printf("The username %s exist already , please try another.\n", msg.proposed_nickname);
    }
    return retval;
  }

int handle_login(india_universal_message msg)
{
  int receive=0;
  char *buffer;
  india_universal_message newmsg= {0};
  int retval=1;
  printf("-------------Login Process----------------------\n");

  Login(msg);
  receive = read_universal_india_message(&newmsg,msg.socket,&buffer);
  printf("message ID :%d\n",newmsg.id);
  printf("error code :%d\n",newmsg.error_code);
  printf("receive value:%d\n",receive);
  if(receive < 0)
  {
      perror("failed to receive");
      exit(1);
  }
  else if(newmsg.error_code == USER_DOES_NOT_EXIST)
  {
    printf(" Username does not match Or It does not exist in our system Press enter to login again \n ");
  //  close(msg.socket);
    retval=0;
  }
	else if (newmsg.error_code ==WRONG_PW){
		printf("Wrong Password\n");
		retval=0;
	}
  return retval;

}


int main(int argc, char *argv[]){
  //check the input in the terminal
  india_universal_message connection;
  //client = malloc(sizeof(client_thread));

  if (argc != 3) {
        printf("Usage: %s [server] [port]\n", argv[0]);
        return 0;
    }
 //n = pthread_create(&(client->thread_id), NULL, input_reader, (void*)client);
 connection_to_server(connection, argv[1], argv[2]);
 //n = pthread_join(client->thread_id, &thread_join);


  return 1;
}
