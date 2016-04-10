
#include "india_messages_protocol.h"

//prints the time info for the chat
void india_message_chat_time(char str [20]){
        struct timeval tv;
        gettimeofday(&tv,0);
        time_t long_time;
        struct tm *newtime;
        time(&long_time);
        newtime = localtime(&long_time);
        //char result[100] = {0};
        sprintf(str, "%02d:%02d:%02d.%03ld", newtime->tm_hour,newtime->tm_min,newtime->tm_sec, (long)tv.tv_usec / 1000);

}
// Removes the  newline character from a string and end of the string caracter for the message printing purpose.
void remove_newline_caracter(char *str)
{
        int len = strlen(str) - 1;
        if (str[len] == '\n')
           {
              str[len] = '\0';
           }
}
// Join and creat chatroom status based on the caracter enter by users, This function return C or J .
char join_creat_chatroom(){
        char resp;
        char c[10];

        while(1)
        {
            printf("Do you want to Creat or Join? press C/c for creat chatroom or J/j for Joining chatroom \n");
            scanf(" %s", c);
            resp=c[0];

               if(resp=='c'||resp=='C')
               {
                   printf("-----Creating Chatroom-------\n");
                   break;
               }
               else if (resp=='J'||resp=='j')
               {
                   printf("Joining Chat room\n");
                   break;
               }

        }
        return resp;

}
//print chat rule
void Chat_Rule()
{
        printf(KGRN"###########################################################\n"RESET);
        printf(KGRN"#          Those are the rules to follow:                 #\n"RESET);
        printf(KGRN"###########################################################\n"RESET);
        printf(KYEL"If You need help  type /help or /h\n");
        printf("To quit type /quit or /q\n");
        printf("To show chat room type /chatrooms or /list\n");
        printf("To show users type /users or /showusers\n");
        printf("/PVM <username> Send a private message to given username\n");
        printf("/Group <Groupname> Send a public message to given groupname\n");
        printf("/EXIT <groupname> To exit a  chat room \n" RESET);

}
//check the weather the user wants to login or register ,this function  return letter l/l or R/r
char user_status(){
        char resp;
        char c[10];

        while(1)
        {
            printf("Do you want to Register or Login? press L/l for login or R/r for Registration \n");
            scanf(" %s",c);
            //fflush(stdin);
             resp=c[0];
             if(resp=='l'||resp=='L')
             {
                puts(KMAG "WELCOME TO LOGIN" RESET);
                 break;
             }
             else if (resp=='r'||resp=='R')
             {
                 puts(KMAG "WELCOME TO REGISTRATION" RESET);
                 break;
             }


        }
       return resp;

}
//function getch caracter that hide the caracter enter from terminal
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
//check the username length and password .
void username_check(char *username,char *password)
{

        while(1)
        {

            fflush(stdout);
            memset(username,0,sizeof(username));
            fgets(username,20, stdin);
            if((int)strlen(username)>10||(int)strlen(username)==1)
            {
               printf("Please:Enter username that is  between 1  to 10 character :\n");

            }
           else
           {
               char str[15],c=' ';int i=0;
               //printf("username lenght %d:", (int)strlen(username));
               //printf("your username is :\n%s",username);
               printf("Enter your password <Max 5 carater>:\n");

               while (i<=6)
               {
                    str[i]=getch();
                    c=str[i];
                    if(c=='\n')
                    {
                        break;
                    }
                    else
                    {
                        printf("*");
                    }
                    i++;
                }
               str[i]='\0';
               remove_newline_caracter(str);
               strcpy(password,str);
               //printf("\npassword is :%s\n",password);
               break;
           }


       }
        remove_newline_caracter(username);
}

//send user information for registration to the server
void register_user(india_universal_message msg)
{

        msg.id =REGISTER_USER;
        char *buffer;
        //printf("register name:%s\n",msg.proposed_nickname);
        //printf("register password:%s\n",msg.password);
        if(send_universal_india_message(msg, msg.socket,buffer) < 0)
        {
          perror("Failed to register to server");
          exit(1);

        }

}
//send user information to server to be  login
void Login(india_universal_message msg)
{
        char *buffer;
        msg.id =LOGIN;
        //printf("Login name:%s\n",msg.proposed_nickname);
        //printf("Login password:%s\n",msg.password);
      	strcpy(msg.sender,msg.proposed_nickname);

        if(send_universal_india_message(msg, msg.socket,buffer) < 0)
        {
            perror("Failed to Login to server");
            exit(1);
        }


}
// close the socket
void close_socket(india_universal_message msg)
{
        close(msg.socket);
        exit(1);
}
void handle_server_message(india_universal_message  * connection)
{
        india_universal_message newmsg={0},msg;
        char *buffer2;
        int val1 = 0;
        char buff[20];
      	while(1)
      	{
          		val1=read_universal_india_message(&newmsg,connection->socket,&buffer2);
          		if(val1 < 0)
            	  {
              			perror("Failed reading from server");
               			exit(1);
            		}
            		else if(val1 == 0)
            		{
              			close(connection->socket);
              			puts("Server disconnected.");
              			exit(0);
            		}

            		switch(newmsg.id)
            		{
              			case PRIVATE_DATA:
                    {
                        if (newmsg.error_code==USER_DOES_NOT_EXIST)
                        {
                            printf("User does not exist \n");
                        }
                        else
                        {
                            //memset(buff,0,20);

                              //time(&end_t);
                              //end_t=clock();
                              //diff_t = difftime(end_t,start_t);
                              //printf("arrival_time: %f",diff_t);
                            india_message_chat_time(buff);
                            printf(KMAG "<< %s >> ",buff);
                     			  printf(KGRN "From %s:  " RESET, newmsg.sender);
			    buffer2[newmsg.data_lenght] = '\0';
                            puts(buffer2);
                            //end_t=0;
                            //diff_t=0;
                        }
                  			break;
                    }

              		 case CHAT_ROOM_DATA:
                    {
                       //memset(buff,0,20);
                        india_message_chat_time(buff);
                        printf(KGRN "<< %s >> " ,buff);
                  		  printf(KMAG "From %s in  <<%s>>:" RESET ,newmsg.sender, newmsg.chat_room_name);
			buffer2[newmsg.data_lenght] = '\0';
                        puts(buffer2);

              			    break;
                    }

                   case JOIN_RESPONSE:
                    {

                      	if(newmsg.error_code==SUCCESS)
                        {
                            puts(KGRN "YOU join successfully the chatroom, You may start chattting now" RESET);

                        }
                        else
                        {
                            puts(KRED "Chatroom or user does not exist to display the available chatroom press /list" RESET);

                        }

                        break;
                    }
                   case CREATE_CHAT_ROOM_RESPONSE :
                    {
                      	if(newmsg.error_code==SUCCESS)
                        {
                            puts(KGRN "Successfully created  chatroom, to start chatting you must join the group" RESET);
                            puts(KGRN "Please Join the group that you have created" RESET);
                        }
                        else
                        {
                          puts(KRED "The chatroom is in use , To display the available chatroom press  /list" RESET);

                        }

                        break;
                   }
                   case EXIT_RESPONSE:
                    {
                        if(newmsg.error_code==SUCCESS)
                        {

                          puts(KGRN "Exit successfully" RESET);
                        }
                        else
                        {
                          puts(KRED "Chatroom or user does exist "RESET);
                        }
                        break;
                   }


              		 default:
                     		 printf("%s\n ",buffer2);
                		     break;
            		}
      	}
}

//user input controler to chat
void control_user_input(india_universal_message connection)
{
        char input[BUFFER_LENGHT];
        char *buffer;
        char *buffer2;
        int val1=0;
        int val=0;
        char *destination;
        char data[BUFFER_LENGHT];
        char *group;
        char resp;
        char buff[20];
        india_universal_message newmsg={0},msg={0},message={0};
        while(1)
        {


            INPUT:
            fgets(input, BUFFER_LENGHT, stdin);
            CHATRULE:
            remove_newline_caracter(input);
            //printf("input enter:%s\n",input);
            if(strcmp(input,"/help") == 0 ||strcmp(input,"/h")==0)
            {
              puts(KMAG "-----------WELCOME TO HELP-----------------" RESET);
                Chat_Rule();
            }
            else if(strcmp(input,"/q") == 0 || strcmp(input,"/quit") == 0)
            {
              puts(KMAG "-----------CONNECTION CLOSE----------------" RESET);
                close_socket(connection);
            }
            else if(strcmp(input,"/chatrooms") == 0 || strcmp(input,"/list") == 0)
            {

                msg.id=SHOW_CHAT_ROOMS;
              puts(KMAG "-----------LIST CHATROOM ---------------------" RESET);
                if(send_universal_india_message(msg, connection.socket,buffer2) < 0)
                {
                    perror("Failed sending to server");
                    exit(1);
                }

            }
            else if(strcmp(input,"/users") == 0 || strcmp(input,"/showusers") == 0)
            {
                msg.id=SHOW_USERS;
              puts(KMAG "--------------USERS--------------------------" RESET);
                if(send_universal_india_message(msg, connection.socket,buffer2) < 0)
                {
                    perror("Failed sending to server");
                    exit(1);
                }

            }
            else if(strncmp(input,"/PVM",4) == 0)
            {
                destination =strtok(input+5," ");
                //printf("PVM");
                //printf(" destination name:%s\n",destination);
                if( destination == NULL)
                {
                    puts(KRED " You enter a NULL , The format for private messages is: /MPV <username> " RESET);
                    Chat_Rule();
                    goto INPUT;
                }

               if(strlen(destination) == 0)
               {
                    puts(KRED "To chat In private you must give a Username." RESET);
                    Chat_Rule();
                    goto INPUT;
               }

              if(strlen(destination) >NAME_LENGHT )
              {
                    puts(KRED "The username must be between 1 and 10 characters." RESET);
                    Chat_Rule();
                    goto INPUT;
              }
              if(strcmp(destination,connection.proposed_nickname)==0)
              {    puts(KRED "Sorry you cannot chat with yourself" RESET);
                   Chat_Rule();
                   goto INPUT;

              }
            	CHAT:
                  	memset(data,0,BUFFER_LENGHT);
                    fgets(data,BUFFER_LENGHT,stdin);
                    remove_newline_caracter(data);
                  	if(data[0]=='/'||data[0]==NULL)
                     {
                    		  strcpy(input,data);
                    		  goto CHATRULE;
                  	 }
                  	 else
                     {

                          msg.id=PRIVATE_DATA;
                          strcpy(msg.receiver,destination);
                          strncpy(msg.sender,connection.proposed_nickname,10);
                          msg.data_lenght=(int)strlen(data);

                          //time(&start_t);
                          //printf("start time: %f\n",start_t);
                          val=send_universal_india_message(msg,connection.socket,data);
                             //printf("send value of val:%d",val);
                          //india_message_chat_time(buff);
                        //  printf(KMAG "<< %s >> ",buff);
                          if(val < 0)
                          {
                              perror("Failed sending to server");
                              exit(1);
                          }
                          //start_t=0;
                      		goto CHAT;

                  		}



            }

            //Joing and creat a chatroom and sending public message
            else if(strncmp(input,"/Group",6) == 0)
            {
                group =strtok(input+7," ");
                //printf("group name:%s\n",group);
                if( group == NULL)
                {
                    puts(KRED " You enter a NULL , The format for Joning group : /Group <groupname> " RESET);
                    Chat_Rule();
                    goto INPUT;
                }

                if(strlen(group) == 0)
                {
                    puts(KRED "To join chat group you must give a name you would like to join." RESET);
                    Chat_Rule();
                    goto INPUT;
                }

                if(strlen(group) > CHAT_ROOM_NAME_LENGHT)
                {
                    puts(KRED "The groupname must be between 1 and 10 characters." RESET);
                    Chat_Rule();
                    goto INPUT;
                }
                resp=join_creat_chatroom();
                if (resp=='c'||resp=='C')
                {
                    msg.id=CREATE_CHAT_ROOM;
                }
                else
                {

                    msg.id=JOIN;
                }

                strcpy(msg.chat_room_name,group);
                strcpy(msg.sender,connection.proposed_nickname);
                val =send_universal_india_message(msg, connection.socket,buffer2);
                if( val< 0)
                {
                    perror("Failed sending to server");
                    exit(1);
                }



            }
            //start group chat
            else if(resp=='j'||resp=='J')
            {

                message.id=CHAT_ROOM_DATA;
                strcpy(message.chat_room_name,group);
                strcpy(message.sender,connection.proposed_nickname);
                  //printf(">>YOU:");
                 //scanf("%s",data);

                GROUP:
                    memset(data,0,BUFFER_LENGHT);
                    fgets(data,BUFFER_LENGHT,stdin);
                    remove_newline_caracter(data);
                      //printf("%c\n",data[0]);
                    if(data[0]=='/'||data[0]==NULL)
                      {

                         strcpy(input,data);
                         resp=" ";
                         goto CHATRULE;

                      }
                      else
                      {
                          message.data_lenght=(int)strlen(data);
                          remove_newline_caracter(data);
                          val =send_universal_india_message(message, connection.socket,data);
                          if( val< 0)
                          {
                              perror("Failed sending to server");
                              exit(1);
                          }
                          goto GROUP;
                    }

        }
            //exit chat room
            else if(strncmp(input,"/EXIT",5) == 0)
            {
                group =strtok(input+6," ");
                printf("%s",group);
                printf("group name:%s\n",group);
                if( group == NULL)
                {
                    puts(KRED " You enter a NULL , The format for exiting the  group : /EXIT <groupname> " RESET);
                    Chat_Rule();
                    goto INPUT;
                }

                if(strlen(group) == 0)
                {
                    puts(KRED "To Exit chat group you must give a name of the group." RESET);
                    Chat_Rule();
                    goto INPUT;
                }

                if(strlen(group) > 20)
                {
                    puts(KRED "The groupname must be between 1 and 20 characters." RESET);
                    Chat_Rule();
                    goto INPUT;
                }
                msg.id=EXIT;
                strcpy(msg.chat_room_name,group);
                strcpy(msg.sender,connection.proposed_nickname);
                val =send_universal_india_message(msg, connection.socket,data);
                printf("value %d:\n",val);
                if( val< 0)
                {
                  perror("Failed sending to server");
                  exit(1);
                }

            }

            else
            {
                puts(KRED "You are out of option please kindly follow our rules " RESET);
            	  Chat_Rule();
                goto INPUT;
            }

      }
}


// setting up connecting to the server
void connection_to_server(india_universal_message msg,char *Ip_addr,char *port_num){
        struct addrinfo hints, *res,*ressave;
        int value;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
        hints.ai_socktype = SOCK_STREAM;

        if ( (value= getaddrinfo(Ip_addr,port_num, &hints, &res)) != 0)
        {
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
        register_or_login(msg);

}
void register_or_login(india_universal_message msg)
{

        int retval = 0;
      	pthread_t serv_msg_thrd;

        while(retval == 0)
        {

            char status=user_status();
            username_check(msg.proposed_nickname,msg.password);
             //printf("the struct username : %s\n",msg.proposed_nickname);
            //printf("the struct password: %s\n",msg.password);

            if (status=='r'|| status=='R')
            {
                retval = handle_registration(msg);
            }
            else{
                retval = handle_login(msg);
            }
          }
         puts(KBLU "\nWELCOME TO INDIAN CHAT APPLICATION" RESET);
      	 Chat_Rule();

      	 pthread_create(&serv_msg_thrd, NULL,(&handle_server_message),(void *)&msg );



         control_user_input(msg);


}


int handle_registration(india_universal_message msg)
{
        int receive=0;
        india_universal_message newmsg = {0};
        char *buffer;
        int retval = 0;

        //printf("-------------Register Process----------------------\n");
        register_user(msg);
        receive = read_universal_india_message(&newmsg,msg.socket,&buffer);
        //printf("message ID :%d\n",newmsg.id);
        //printf("error code :%d\n",newmsg.error_code);
        //printf("receive value:%d\n",receive);

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
            puts(KRED "The username exist already , please try another" RESET);
            //printf("The username exist already , please try another.\n", msg.proposed_nickname);
        }
        return retval;
  }

int handle_login(india_universal_message msg)
{
        int receive=0;
        char *buffer;
        india_universal_message newmsg= {0};
        int retval=1;
        //printf("-------------Login Process----------------------\n");

        Login(msg);
        receive = read_universal_india_message(&newmsg,msg.socket,&buffer);
      //  printf("message ID :%d\n",newmsg.id);
        //printf("error code :%d\n",newmsg.error_code);
        //printf("receive value:%d\n",receive);
        if(receive < 0)
        {
            perror("failed to receive");
            exit(1);
        }
        else if(newmsg.error_code == USER_DOES_NOT_EXIST)
        {
            puts(KRED "Username does not match Or It does not exist in our system" RESET);
            retval=0;
        }
        else if (newmsg.error_code ==WRONG_PW)
        {
            puts(KRED "Wrong Password" RESET);
            retval=0;
        }
        return retval;

}


int main(int argc, char *argv[]){

        india_universal_message connection;


        if (argc != 3)
        {
            printf("Usage: %s [server] [port]\n", argv[0]);
            return 0;
        }


       connection_to_server(connection, argv[1], argv[2]);



        return 1;
}
