

#include "india_messages_protocol.h"


#define MAXLINE 80
#define LISTENQ 10

/* new defines */
#define MAX_USER_COUNT 100




/***global variables in server***/
struct user *root_user;
struct chatroom *root_chatroom;

int showServerIPs(void)
{
	struct ifaddrs *ifaddr, *ifa;
        int family, s, n;
        char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               return -1;
           }

           /* Walk through linked list, maintaining head pointer so we
              can free list later */

           for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next, n++) 
		{
               		if (ifa->ifa_addr == NULL)
                   		continue;

               		family = ifa->ifa_addr->sa_family;
	
              		 /* For an AF_INET* interface address, display the address */

               if (family == AF_INET || family == AF_INET6) 
			{
                   		s = getnameinfo(ifa->ifa_addr,
                           	(family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
                           	host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                   		if (s != 0) 
					{
                       				printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       				return -1;
                   			}
				
                  		printf("\t\taddress: <%s>\n", host);

               } 
		
           }
  
       	freeifaddrs(ifaddr);
}

struct user 
	{
		char name[NAME_LENGHT];
		char pw[PW_LENGHT];
		int login;
		int connfd;
		struct user *next;
	};

int register_user(char name[NAME_LENGHT], char pw[PW_LENGHT], int connfd)
{

	struct user *conductor;
	conductor = root_user;
	char test_name[NAME_LENGHT];
	int found = 0;

/* search if name exist already */ 

    	if ( conductor != 0 ) 
		{
			strcpy(&test_name[0], conductor->name);
			while (strcmp(test_name, name) != 0)
				{
					conductor = conductor->next;
					if (conductor == 0)
						{
							found = 0;
							break;
						}
					else
						{
							strcpy(&test_name[0], conductor->name);
						}
				}

			if(strcmp(test_name, name) == 0) // zero == names are same
				{
					found = 1;
				}
		}

	if (found)
		{
			return NICK_NAME_IN_USE;
		}
	

/* add new user */
	conductor = root_user;

    	if ( conductor != 0 ) 
		{
        		while ( conductor->next != 0)
        			{
            				conductor = conductor->next;
        			}
    		}
	    /* Creates a node at the end of the list */
    	conductor->next = malloc( sizeof(struct user) );  

    	conductor = conductor->next; 
	if ( conductor == 0 )
    		{
        		//printf( "Out of memory when adding new users" );
        		return 99;
    		}	
	
	conductor->next = 0;

	strcpy(conductor->name, &name[0]);
	strcpy(conductor->pw, &pw[0]);
	conductor->connfd = connfd;
	conductor->login = 1;

	return SUCCESS; 
}

int login_user(char name[NAME_LENGHT], char pw[PW_LENGHT], int connfd)
{

	struct user *conductor;
	conductor = root_user;
	char test_name[NAME_LENGHT];
	char test_pw[PW_LENGHT];
	uint32_t i;


	conductor = conductor->next;

    	if ( conductor == 0 )
		{
			//printf("conductor == 0\n");
			 return USER_DOES_NOT_EXIST;
		}

	for ( i=0 ; i < MAX_USER_COUNT ; i++)
		{
			strcpy(&test_name[0], conductor->name);
			if (strcmp(test_name, name) != 0)
				{
					conductor = conductor->next;
					if (conductor == 0)
						{
							//printf("2. conductor == 0\n");
							return USER_DOES_NOT_EXIST;
						}
					continue;
				}
			else // name found
				{
					strcpy(&test_pw[0], conductor->pw);
					if ( strcmp(test_pw, pw) == 0) // pw matches
						{
							conductor->login = 1;
							conductor->connfd = connfd;
							return SUCCESS;
						}
					else // PW does not match
						{
							return WRONG_PW; // pw does not match
						}
				}
			} 

	/* should not get here except if more users that max count */
	
	//printf("should not get here except if more users that max count, i= %d\n", i);

	return USER_DOES_NOT_EXIST;
}
	
int show_users(char **text_ptr_out, struct india_universal_message *msg_out) 
{
	struct user *conductor;
	conductor = root_user;
	char test_name[NAME_LENGHT];
	int found = 0;

	char *ptr_to_mem;
	int temp;
	uint32_t memory_size_needed = 0;
	uint32_t location = 0;

/* search existing names and summing up the size */ 
	conductor = conductor->next;

    	while ( conductor != 0 ) 
		{
			memory_size_needed = memory_size_needed + strlen(conductor->name) + sizeof('\n'); // ei ihan oiken
			//memory_size_needed = memory_size_needed + strlen(conductor->name);
			conductor = conductor->next;
		}

	if (memory_size_needed == 0)
		{
			msg_out->data_lenght = 0;
			return USER_DOES_NOT_EXIST;
		}

	ptr_to_mem = (char *) malloc(memory_size_needed);
	msg_out->data_lenght = memory_size_needed;

/* search existing names and add to the users list  */ 

	conductor = root_user;  // start from the beginning
	conductor = conductor->next;
	location = 0;
	while ( conductor != 0 )
		{
			strcpy(&ptr_to_mem[location], conductor->name); 
			location = location+strlen(conductor->name);
			strcpy(&ptr_to_mem[location], "\n");
			location = location + strlen("\n");
			conductor = conductor->next;
		}
	*text_ptr_out = ptr_to_mem;			
	return SUCCESS;
}

struct participant
	{
		char name[NAME_LENGHT];
		struct user *ptr_to_user;
		struct participant *next_member;
	};
		
struct chatroom
	{
		char name[CHAT_ROOM_NAME_LENGHT];
		struct chatroom *next_chatroom;
		struct participant *member;
	};

int exit_chatroom(char cr[CHAT_ROOM_NAME_LENGHT], char nn[NAME_LENGHT])
{

/* Step1: find chatroom */

	struct chatroom *cr_conductor;
	cr_conductor = root_chatroom;
	char test_chatroom_name[CHAT_ROOM_NAME_LENGHT];
	int found = 0;


	cr_conductor = cr_conductor->next_chatroom; // because "root" does not have members 
	found = 0;

    	if ( cr_conductor != 0 ) 
		{
			strcpy(&test_chatroom_name[0], cr_conductor->name);
			while (strcmp(test_chatroom_name, cr) != 0)
				{
					cr_conductor = cr_conductor->next_chatroom;
					if (cr_conductor == 0)
						{
							found = 0;
							break;
						}
					else
						{	
							found = 0;
							strcpy(&test_chatroom_name[0], cr_conductor->name); 
							continue;
						}
				}
			if(strcmp(test_chatroom_name, cr) == 0) 
				{
					found = 1; // correct chat room found. Note: cr_conductor points to that chat room now
				 }
		 }

	if( found == 0)
		{
			return CHAT_ROOM_DOES_NOT_EXIST;
		} 

/* Step 3: find user "participant" item */

	struct participant *participant_conductor;
	struct participant *participant_conductor_previous;

	participant_conductor_previous = cr_conductor->member;
	participant_conductor = cr_conductor->member;  // points now to chatroom's first member
		
	char test_name[NAME_LENGHT];

	found = 0;
	while(participant_conductor !=0)
		{
			//check participant name vs given name
			//copy name for compare
			strcpy(&test_name[0],participant_conductor->name);

			if (strcmp(test_name, nn) == 0) // same name found
				{
					/* Step 4: connect previous to the next */
					participant_conductor_previous->next_member = participant_conductor->next_member;
					found = 1;
					break;
				}
			else
				{
					participant_conductor_previous = participant_conductor;
					// go to next participant
					participant_conductor = participant_conductor->next_member;
					continue;
				}
		}

	if( found)
		{
			/* Step 5: free memory of the removed participant */
			free(participant_conductor);
		} 

	return SUCCESS;			
}

int  join_chatroom(char cr[CHAT_ROOM_NAME_LENGHT], char nn[NAME_LENGHT])
{

/* Step1: find user and ptr to it */

 	struct user *usr_conductor;
	usr_conductor = root_user;
	char test_name[NAME_LENGHT];
	char test_pw[PW_LENGHT];
	uint32_t i; 						// used in for loop
	struct user *ptr_to_user;
	int found = 0;


	usr_conductor = usr_conductor->next;

    	if ( usr_conductor == 0 )
		{
			//printf("usr_conductor == 0\n");
			 return USER_DOES_NOT_EXIST;
		}

	for ( i=0 ; i < MAX_USER_COUNT ; i++)
		{
			strcpy(&test_name[0], usr_conductor->name);
			if (strcmp(test_name, nn) != 0)
				{
					usr_conductor = usr_conductor->next;
					if (usr_conductor == 0)
						{
							//printf("2. usr_conductor == 0\n");
							return USER_DOES_NOT_EXIST;
						}
					continue;
				}
			else // name found
				{
					ptr_to_user = usr_conductor; // get ptr to user
					found = 1;
				}
		} 

	if( found == 0)
		{
			/* should not get here except if more users that max count */
			//printf("should not get here except if more users that max count in join(), i= %d\n", i);
			return USER_DOES_NOT_EXIST;
		} 


/* Step 2: find correct chatroom to join */

	struct chatroom *cr_conductor;
	cr_conductor = root_chatroom;
	char test_chatroom_name[CHAT_ROOM_NAME_LENGHT];
	found = 0;


	cr_conductor = cr_conductor->next_chatroom; // because "root" does not have members 
	found = 0;

    	if ( cr_conductor != 0 ) 
		{
			strcpy(&test_chatroom_name[0], cr_conductor->name);

			while (strcmp(test_chatroom_name, cr) != 0)
				{
					cr_conductor = cr_conductor->next_chatroom;
					if (cr_conductor == 0)
						{
							found = 0;
							break;
						}
					else 
						{	
							found = 0;
							strcpy(&test_chatroom_name[0], cr_conductor->name); 

							continue;
						}
				}

			if(strcmp(test_chatroom_name, cr) == 0) 
				{
					found = 1; // correct chat room found. Note: cr_conductor points to that chat room now
				 }
		}

	if( found == 0)
		{
			return CHAT_ROOM_DOES_NOT_EXIST;
		} 

/* Step 3: search if already joined */

	struct participant *participant_conductor;
	participant_conductor = cr_conductor->member;  // points now to chatroom's first member
	
	found = 0;
	while(participant_conductor !=0)
		{
			//check participant name vs given name
			//copy name for compare
			strcpy(&test_name[0],participant_conductor->name);

			if (strcmp(test_name, nn) == 0) // same name found
				{
					// this is ok, user already joined
					found = 1;
					break;
				}
			else
				{
					// go to next participant
					participant_conductor = participant_conductor->next_member;
					continue;
				}
		}

	if( found)
		{
			return SUCCESS;			
		} 


/* Step 4: add new member */

//	printf("/* Step 4: add new member */\n");

 	participant_conductor = cr_conductor->member;  // points now to chatroom's first member

//	printf("Name of chatroom pointed: %s\n", cr_conductor->name);

    	if ( participant_conductor != 0 ) 
		{
        		while ( participant_conductor->next_member != 0)
        			{
            				participant_conductor = participant_conductor->next_member;
        			}

			// second of more			
	    		/* Creates a node at the end of the list */
	    		participant_conductor->next_member = (struct participant *) malloc( sizeof(struct participant) ); 
		    	participant_conductor = participant_conductor->next_member; 

			if ( participant_conductor == 0 )
    				{
        				//printf( "Out of memory when adding new members" );
        				return 99;
    				}	

			participant_conductor->next_member = 0; // initialize next to zero
			strcpy(participant_conductor->name, &nn[0]); // add name
//			printf("added participant:%s\n", participant_conductor->name);    // tähän ei tulla
			participant_conductor->ptr_to_user = ptr_to_user; // add ptr to user
			return SUCCESS; 
    		}
	else		// first item 
		{
		    	participant_conductor = (struct participant *) malloc( sizeof(struct participant) ); 
			cr_conductor->member = participant_conductor;
//		    	participant_conductor = participant_conductor->next_member; 

			if ( participant_conductor == 0 )
    				{
        				//printf( "Out of memory when adding new members" );
        				return 99;
    				}	

			participant_conductor->next_member = 0; // initialize to zero
			strcpy(participant_conductor->name, &nn[0]); // add name
//			printf("added participant:%s\n", participant_conductor->name);    // tullaanko tähän?
			participant_conductor->ptr_to_user = ptr_to_user; // add ptr to user

			return SUCCESS; 
		}


	//printf("PROGRAM EXECUTION SHOULD NOT COME HERE!\n");
	
	return 77;
}


int create_chat_room(char cr[CHAT_ROOM_NAME_LENGHT])
{
	struct chatroom *conductor;
	conductor = root_chatroom;
	char test_chatroom_name[CHAT_ROOM_NAME_LENGHT];
	int found = 0;

	conductor = root_chatroom->next_chatroom;

/* search if name exist already */ 

    	if ( conductor != 0 ) 
		{
			strcpy(&test_chatroom_name[0], conductor->name);
			while (strcmp(test_chatroom_name, cr) != 0)
				{
					conductor = conductor->next_chatroom;
					if (conductor == 0)
						{
							found = 0;
							break;
						}
					else
						{
							strcpy(&test_chatroom_name[0], conductor->name);
						}
				}

			if(strcmp(test_chatroom_name, cr) == 0) // zero == names are same
				{
					found = 1;
				}
		}

	if (found)
		{
			return CHAT_ROOM_NAME_IN_USE;
		}


/* add new chat room */
	conductor = root_chatroom;

    	if ( conductor != 0 ) 
		{
        		while ( conductor->next_chatroom != 0)
        			{
            				conductor = conductor->next_chatroom;
        			}
    		}
	    /* Creates a node at the end of the list */
    	conductor->next_chatroom = malloc( sizeof(struct chatroom) );  

    	conductor = conductor->next_chatroom; 
	if ( conductor == 0 )
    		{
        		//printf( "Out of memory when adding new chat rooms" );
        		return 99;
    		}	
	
	conductor->next_chatroom = 0;
	conductor->member = 0;

	strcpy(conductor->name, &cr[0]);
	return SUCCESS; 
}

int show_chat_rooms( char **text_ptr_out, struct india_universal_message *msg_out)
{
	struct chatroom *conductor;
	conductor = root_chatroom;
	char test_chatroom_name[CHAT_ROOM_NAME_LENGHT];
	int found = 0;
	uint32_t laskuri; 

	char *ptr_to_mem;
	int temp;
	uint32_t memory_size_needed = 0;
	uint32_t location = 0;

/* search existing names and summing up the size */ 
	conductor = conductor->next_chatroom;

	laskuri = 0;
    	while ( conductor != 0 ) 
		{
			memory_size_needed = memory_size_needed + strlen(conductor->name) + strlen("\n");
			conductor = conductor->next_chatroom;
			laskuri++;
		}

	if (memory_size_needed == 0)
		{
			msg_out->data_lenght = 0;
			return CHAT_ROOM_DOES_NOT_EXIST;
		}

	ptr_to_mem = (char *) malloc(memory_size_needed);
	msg_out->data_lenght = memory_size_needed;
	msg_out->number_of_chat_rooms = laskuri;

/* search existing names and add to the users list  */ 

	conductor = root_chatroom;  // start from the beginning
	conductor = conductor->next_chatroom;
	location = 0;
	while ( conductor != 0 )
		{
			strcpy(&ptr_to_mem[location], conductor->name); 
			location = location+strlen(conductor->name);
			strcpy(&ptr_to_mem[location], "\n");
			location = location + strlen("\n");
			conductor = conductor->next_chatroom;
		}
	*text_ptr_out = ptr_to_mem;			
	return SUCCESS;

}

int forward_private_data(char sn[NAME_LENGHT], char rn[NAME_LENGHT], char* ptr, int lenght)
{

// sn is the sender's name
// rn is the receiver's name

/* step 1 find receiver */

	struct user *conductor;
	conductor = root_user;
	char test_name[NAME_LENGHT];
	struct india_universal_message msg;
	int sockfd;

	conductor = conductor->next;

    	if ( conductor == 0 )
		{
			 return USER_DOES_NOT_EXIST;
		}

	while( conductor != 0 )
		{
			strcpy(&test_name[0], conductor->name);
			if (strcmp(test_name, rn) != 0)
				{
					conductor = conductor->next;
					if (conductor == 0)
						{
							//printf("2. conductor == 0 i.e. USER_DOES_NOT_EXIST\n");
/// add sending private response
							return USER_DOES_NOT_EXIST;
						} 
					continue;
				}
			else // name found
				{
					msg.id = PRIVATE_DATA;
					msg.error_code = SUCCESS;
					msg.data_lenght = lenght;
					strcpy(msg.receiver, conductor->name);
					strcpy(msg.sender, &sn[0]);
					//take connfd for send message function
					sockfd = conductor->connfd; 
					//printf("sending private data to %s, socket %d\n", conductor->name,sockfd); 
					//forward message
					send_universal_india_message( msg, sockfd, ptr );
					return SUCCESS;
				}
		} 

	/* should not get here ! */
	
	//printf("sending private data error, should not get here!");

	return USER_DOES_NOT_EXIST;

}


int forward_chat_room_data(char nn[NAME_LENGHT],char cr[CHAT_ROOM_NAME_LENGHT], char* ptr, int lenght)
 {

// nn is sender name
// cr is the chatroom name is used to find users (members) to whom this message shall be forwarded  
// 

/* Step 1: find correct chatroom  */

	struct chatroom *cr_conductor;
	cr_conductor = root_chatroom;
	char test_chatroom_name[CHAT_ROOM_NAME_LENGHT];
	int found = 0;


	cr_conductor = cr_conductor->next_chatroom; // because "root" does not have members 
	found = 0;

    	if ( cr_conductor != 0 ) 
		{
			strcpy(&test_chatroom_name[0], cr_conductor->name);

			while (strcmp(test_chatroom_name, cr) != 0)  //names do not match
				{
					cr_conductor = cr_conductor->next_chatroom;
					if (cr_conductor == 0)
						{
							found = 0;
							break;
						}
					else 
						{	
							found = 0;
							strcpy(&test_chatroom_name[0], cr_conductor->name); 
							continue;
						}
				}

			if(strcmp(test_chatroom_name, cr) == 0) 
				{
					found = 1; // correct chat room found. Note: cr_conductor points to that chat room now
//					printf("chatroom found: %s\n", cr_conductor->name);
//					printf("chatroom member 1:%s\n", cr_conductor->member->name);
//					printf("chatroom member 2:%s\n", cr_conductor->member->next_member->name);
				 }
		}

	if( found == 0)
		{
			return CHAT_ROOM_DOES_NOT_EXIST;
		} 

/* Step 2: find members and forward message  */
// this function is used:
// int send_universal_india_message( struct india_universal_message msg, int sockfd, char* ptr )

	struct india_universal_message msg;
	char test_name[NAME_LENGHT];
	strcpy(msg.chat_room_name, &cr[0]);
	strcpy(msg.sender, &nn[0]); // who has send the message to the chatroom

	msg.id = CHAT_ROOM_DATA;
	msg.data_lenght = lenght;

	struct participant *participant_conductor;
	participant_conductor = cr_conductor->member;  // points now to chat to room's first member

	int sockfd;
	int retValue = 88;

	while(participant_conductor !=0)
		{

			// do not send back to sender!
			strcpy(&test_name[0],participant_conductor->name);

			if (strcmp(test_name, nn) == 0) // same name found
				{
					// skip this member
					participant_conductor = participant_conductor->next_member;	
					continue;
				}
			//take name to receiver
			strcpy(msg.receiver, participant_conductor->name);

			//take connfd for send message function
			//pitäisi tsekata login status
			if(participant_conductor->ptr_to_user->login == 1)
				{
					sockfd = participant_conductor->ptr_to_user->connfd; 
					//forward message
					retValue = send_universal_india_message( msg, sockfd, ptr );
					if(retValue <1)
						{
							// user not available at the moment
							participant_conductor->ptr_to_user->login = 0;
							//printf("User %s has left\n", participant_conductor->ptr_to_user->name);
						}
				}
			// go to next participant
			participant_conductor = participant_conductor->next_member;	
		}
return SUCCESS;
}

int logoff(char nn[NAME_LENGHT])
{

	struct user *conductor;
	conductor = root_user;
	char test_name[NAME_LENGHT];


	conductor = conductor->next;

    	if ( conductor == 0 )
		{
			//printf("conductor == 0\n");
			 return USER_DOES_NOT_EXIST;
		}

	do
		{
			strcpy(&test_name[0], conductor->name);
			if (strcmp(test_name, nn) != 0)
				{
					conductor = conductor->next;
					if (conductor == 0)
						{
							//printf("logoff: user does not exist!\n");
							return USER_DOES_NOT_EXIST;
						}
					continue;
				}
			else // name found
				{
					conductor->login = 0;
					conductor->connfd = 0;
					return SUCCESS;
				}

		} while (strcmp(test_name, nn) != 0);

// should not get here
	return USER_DOES_NOT_EXIST;
}

void handle_clients(int connfd)
{
					
	struct india_universal_message 	msg;
	struct india_universal_message 	*msg_ptr;
	struct india_universal_message 	msg_out;
	char 							buff[80];
	int 								val;
	char 							*text_ptr;
	char 							*text_ptr_out;
	char 							nn[NAME_LENGHT];
	char 							rn[NAME_LENGHT];
	char 							pw[PW_LENGHT];
	char 							cr[CHAT_ROOM_NAME_LENGHT];

	
	msg_ptr = &msg;

	for(;;)
		{
			val = read_universal_india_message( msg_ptr,  connfd, &text_ptr );

			if(val <1)
				{ 
//					printf("client has closed connection, val: %d\n", val);
					close(connfd);
					break;
					/* should log-off? */
				}	

			switch (msg.id)
				{
					case REGISTER_USER:
						strcpy(nn, msg.proposed_nickname);
						strcpy(pw, msg.password);
						//printf("message received:%d, proposed name: %s, password; %s\n", msg.id, &nn[0], &pw[0]);
						msg_out .id =  REGISTER_USER_RESPONSE;
						msg_out.error_code = register_user(nn, pw, connfd);
						send_universal_india_message( msg_out, connfd, text_ptr_out);
					break;
																	
					case LOGIN :
						strcpy(nn, msg.sender);
						strcpy(pw, msg.password);
						//printf("message received:%d, name: %s, password; %s\n", msg.id, &nn[0], &pw[0]);
						msg_out .id =  LOGIN_RESPONSE;	
						msg_out.error_code = login_user(nn, pw, connfd);
						send_universal_india_message( msg_out, connfd, text_ptr_out);
					break;

					case SHOW_CHAT_ROOMS :
						msg_out .id =  SHOW_CHAT_ROOMS_RESPONSE;
						//printf("message received:%d\n", msg.id);
						msg_out.error_code = show_chat_rooms(&text_ptr_out, &msg_out); 
						send_universal_india_message( msg_out, connfd, text_ptr_out);
					break;
							
					case SHOW_USERS :
						//printf("message received:%d\n", msg.id);
						msg_out .id = SHOW_USERS_RESPONSE;
						int ecode = 0;
						ecode = show_users(&text_ptr_out, &msg_out); 
						msg_out.error_code = ecode;
						send_universal_india_message( msg_out, connfd, &text_ptr_out[0]);
					break;
				
					case JOIN :

						strcpy(nn, msg.sender);
						strcpy(cr, msg.chat_room_name);

						//printf("message received:%d, sender:%s, chatroom:%s\n", msg.id, nn, cr);
						msg_out .id =  JOIN_RESPONSE;
						strcpy(nn, msg.sender);
						strcpy(cr, msg.chat_room_name);
						msg_out.error_code =  join_chatroom(cr,nn);
						msg_out.data_lenght = 0;
						send_universal_india_message( msg_out, connfd, text_ptr_out);
					break;
						
					case EXIT :
						strcpy(nn, msg.sender);
						strcpy(cr, msg.chat_room_name);

						//printf("message received:%d, sender:%s, chatroom:%s\n", msg.id, nn, cr);
						msg_out .id = EXIT_RESPONSE;
						strcpy(nn, msg.sender);
						strcpy(cr, msg.chat_room_name);
						msg_out.error_code =  exit_chatroom(cr,nn);
						msg_out.data_lenght = 0;
						send_universal_india_message( msg_out, connfd, text_ptr_out);
					break;
						
					case CREATE_CHAT_ROOM :
						msg_out .id = CREATE_CHAT_ROOM_RESPONSE;
						strcpy(cr, msg.chat_room_name);
						msg_out.data_lenght = 0;
						msg_out.error_code = create_chat_room(cr);
						//printf("message received:%d proposed chat  room name: %s\n", msg.id, &cr[0]);
						send_universal_india_message( msg_out, connfd, text_ptr_out);
					break;
							
					case CHAT_ROOM_DATA :
						msg_out .id = CHAT_ROOM_DATA;
						strcpy(nn, msg.sender);
						strcpy(cr, msg.chat_room_name);
						msg_out.data_lenght = msg.data_lenght;
						//printf("message received: %d chat  room: %s data: %s\n", msg.id, &cr[0], &text_ptr[0]);
						forward_chat_room_data(nn,cr,text_ptr, msg.data_lenght);
						free(text_ptr);
					break;
							
					case PRIVATE_DATA :

						msg_out .id = PRIVATE_DATA;
						strcpy(nn, msg.sender);
						strcpy(rn, msg.receiver);
						msg_out.data_lenght = msg.data_lenght;
//						printf("message received: %d sender: %s lenght %d, data: %s\n", msg.id, &nn[0], msg_out.data_lenght, &text_ptr[0]);
						msg_out .error_code = forward_private_data(nn, rn, text_ptr, msg_out.data_lenght);
						free(text_ptr);
						// forwarding data was not successful
						if(msg_out .error_code != SUCCESS)
							{
//								msg_out .error_code = USER_DOES_NOT_EXIST;
								msg_out .id = PRIVATE_DATA;
								strcpy(msg_out.receiver, rn);
								send_universal_india_message(msg_out, connfd, text_ptr_out);
							}
						break;

					case EXIT_FROM_SERVICE :
						strcpy(nn, msg.sender);
						logoff(nn);
					break;

					default :
							;
						/* handling for messed-up stuff */
						
				} // end of switch msg.id 
		} // end of for loop					
}

void *start_routine(void *connfd)
{
	pthread_detach(pthread_self());
	handle_clients(((int) connfd));
	close(((int) connfd));

//printf("\nTHREAD END\n");

	return(NULL);
}


int main(void)
{

	int 	debug = 0;
	int 	listenfd = 0;

	root_user = (struct user *) malloc(sizeof(struct user));
	memset(root_user, 0, sizeof(struct user));
	root_user->next = 0;

	root_chatroom = (struct chatroom *) malloc(sizeof(struct chatroom));
	memset(root_chatroom, 0, sizeof(struct chatroom));
	root_chatroom->next_chatroom = 0;
	root_chatroom->member = 0;


	if ((listenfd = socket(AF_INET6, SOCK_STREAM, 0)) < 0) 
		{
			perror("socket");
			return -1;
		}
	
	if (debug) { printf("listenfd: %d\n", listenfd); }

    	// Pick a port and bind socket to it.

	// ask port number from user 
	int	port = 0;

	printf("Give port number for server: ");
	scanf ("%d",&port);

	struct sockaddr_in6 	myservaddr; // cliaddr; siirrettiin cliaddr functioon	

    	// Accept connections from any address.

    	memset(&myservaddr, 0, sizeof(myservaddr));
    	myservaddr.sin6_family = AF_INET6;
    	myservaddr.sin6_addr = in6addr_any;
	myservaddr.sin6_port = htons(port);

    	if (bind(listenfd, (struct sockaddr *) &myservaddr, sizeof(myservaddr)) < 0) { perror("bind"); return -1; }

	if (debug) { printf("BIND OK\n"); } 

	// Set the socket to passive mode, with specified listen queue size

	if (listen(listenfd, LISTENQ) < 0) { perror("listen"); return -1; }

	// tell user used address to configure client

	struct sockaddr_in6* pV6Addr = (struct sockaddr_in6*)&myservaddr;
	struct in6_addr ipAddr       = pV6Addr->sin6_addr;

	//and getting a string is almost identical to IPV4

	char str[INET6_ADDRSTRLEN];
	inet_ntop( AF_INET6, &ipAddr, str, INET6_ADDRSTRLEN );
	printf("\n***************************************");
	printf( "\nServer listens port %d, \nit has following network interfaces:\n", port );
	showServerIPs();
	printf("\n***************************************");
	printf("\n\n\n******** SERVER IN NOW RUNNING AS DAEMON ********\n\n\n");


	pid_t pid, sid;
        
        /* Fork off the parent process */
        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }
        /* If we got a good PID, then
           we can exit the parent process. */
        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }
        
        /* Change the file mode mask */
        umask(0);       
        
        /* Open any logs here */
        
        /* Create a new SID for the child process */
        sid = setsid();
        if (sid < 0) {
                /* Log any failure here */
                exit(EXIT_FAILURE);
        }
        
        /* Change the current working directory THIS IS NOT DONE YET
        if ((chdir("/")) < 0) {
                // Log any failure here
                exit(EXIT_FAILURE);
        }
        */
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);



	

	int					connfd;
	struct sockaddr_in6 	cliaddr;	
	socklen_t			len;
	void				*start_routine(void *);
	pthread_t			thread_id;
	char 				buff[80];


	len = sizeof(cliaddr);

	// Start waiting for incoming connections, until user interrupts

	for ( ; ; ) 
		{
			memset(&cliaddr, 0, sizeof(cliaddr));

			if (	(connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len)	) < 0) 
				{ 
					if (errno == EINTR)
						{
							perror("accept");
							continue;	/* back to for() */  
						}
					perror("accept");
					return connfd; 
				}

			// printf("connection %s, port %d, connfd %d\n", inet_ntop(AF_INET6, &cliaddr.sin6_addr, buff, sizeof(buff)), ntohs(cliaddr.sin6_port), connfd);

			
        		
			if ( pthread_create(&thread_id, NULL, &start_routine, (void *)connfd) != 0 ) 
				{
					perror("pthread_create");
					return -1;
				}

    		} 

	close(listenfd);

	return 1;

 }


