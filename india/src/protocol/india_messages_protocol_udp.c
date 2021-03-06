/******
INDIA MESSAGE HEADER
******/
# include "india_messages_protocol.h"

/******
FUNCTIONS
******/

// SENDING FUNCTION

/* fill necessary universal struct fields for sending */
/* give socket fd */
/* give ptr for data buffer if needed */
  
int sendto_universal_india_message( struct india_universal_message msg, int sockfd, char* ptr)
{

// send msg ID

	int retValue;
	int id = htons(msg.id);
	struct sockaddr_in in_address;
	int inlen = sizeof(in_address);
	
	retValue = sendto(sockfd, &id, sizeof(id), 0, (struct sockaddr *) &in_address, inlen);

	if(retValue < 1)
		{
			printf("India_messages_protocol: sendto retValue: %d\n", retValue);
			return -1;
		}


// handling of client messages sending to server 

	if (msg.id ==  REGISTER_USER)
		{
			sendto(sockfd, &msg.proposed_nickname[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &msg.password[0], PW_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  LOGIN)
		{
			sendto(sockfd, &msg.sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &msg.password[0], PW_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  SHOW_CHAT_ROOMS)
		{
			return 1; // OK			
		}

	if (msg.id ==  SHOW_USERS)
		{
			return 1; // OK			
		}

	if (msg.id ==  JOIN)
		{
			sendto(sockfd, &msg.sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  EXIT)
		{
			sendto(sockfd, &msg.sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  CREATE_CHAT_ROOM)
		{
			sendto(sockfd, &msg.sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

// messages used in both direction

	int temp;
	

	if (msg.id ==  CHAT_ROOM_DATA)
		{
			temp = htons(msg.data_lenght);
			sendto(sockfd, &msg.sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &ptr[0], msg.data_lenght, 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  PRIVATE_DATA)
		{
			temp = htons(msg.data_lenght);
			sendto(sockfd, &msg.sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &msg.receiver[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			sendto(sockfd, (const void *) &ptr[0], msg.data_lenght, 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

// handling of server messages sending to client

	if (msg.id ==  REGISTER_USER_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  LOGIN_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  SHOW_CHAT_ROOMS_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			if (msg.data_lenght == 0)
				{
					// no chat rooms available
					return 1; // OK			
				}
			else
				{
					temp = htons(msg.data_lenght);
					sendto(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
					sendto(sockfd, (const void *) &ptr[0], msg.data_lenght, 0, (struct sockaddr *) &in_address, inlen);
					return 1;
				}
		}

	if (msg.id ==  SHOW_USERS_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			if (msg.data_lenght == 0)
				{
					// no users available
					return 1; // OK			
				}
			else
				{
					temp = htons(msg.data_lenght);
					sendto(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
					sendto(sockfd, (const void *) &ptr[0], msg.data_lenght, 0, (struct sockaddr *) &in_address, inlen);
					return 1;
				}

		}

	if (msg.id ==  JOIN_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  EXIT_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  CREATE_CHAT_ROOM_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			return 1; // OK			
		}

	if (msg.id ==  PRIVATE_DATA_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			if (msg.error_code != SUCCESS)
				{
					sendto(sockfd, &msg.receiver[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen); // who was not availble
				}
			return 1; // OK
		}			

	if (msg.id ==  CHAT_DATA_RESPONSE)
		{
			sendto(sockfd, &msg.error_code, sizeof(int), 0, (struct sockaddr *) &in_address, inlen);
			if (msg.error_code != SUCCESS)
				{
					sendto(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen); // chat room name which was not availble
				}
			return 1; // OK
		}			


	// message not known 
	printf("message id: %d is not known!\n", msg.id); 
	return 0; // 
}


// recvfromING FUNCTION

/* create empty message struct, function will add relevant fields to it*/
/* give ptr for data buffer */

int recvfrom_universal_india_message( struct india_universal_message* msg,  int sockfd, char **ptr)
{

// recvfrom msg ID

	int id = 0;
	int retValue = 0;
	struct sockaddr_in in_address;
	int inlen = sizeof(in_address);
	
	retValue = recvfrom(sockfd, &id, sizeof(id), 0, (struct sockaddr *) &in_address, &inlen);

	if(retValue <1)
		{
//			printf("India_messages_protocol: recvfrom retValue: %d\n", retValue);
			perror("recvfrom Error:");
			return -1;
		}

	msg->id = ntohs(id);
	char *ptr_to_mem;
	int temp;

// handling of client messages recvfroming in server 

	if (msg->id ==  REGISTER_USER)
		{
			recvfrom(sockfd, &msg->proposed_nickname[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &msg->password[0], PW_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);

			return 1; // OK			
		}

	if (msg->id ==  LOGIN)
		{
			recvfrom(sockfd, &msg->sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &msg->password[0], PW_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

	if (msg->id ==  SHOW_CHAT_ROOMS)
		{
			return 1; // OK			
		}

	if (msg->id ==  SHOW_USERS)
		{
			// msg ID is enough
			return 1; // OK			
		}

	if (msg->id ==  JOIN)
		{
			recvfrom(sockfd, &msg->sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

	if (msg->id ==  EXIT)
		{
			recvfrom(sockfd, &msg->sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

	if (msg->id ==  CREATE_CHAT_ROOM)
		{
			recvfrom(sockfd, &msg->sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

// messages used in both direction

		

	if (msg->id ==  CHAT_ROOM_DATA)
		{
			recvfrom(sockfd, &msg->sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			msg->data_lenght = ntohs(temp);
			if(msg->data_lenght >0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					recvfrom(sockfd, &ptr_to_mem[0], msg->data_lenght, 0, (struct sockaddr *) &in_address, &inlen);
					*ptr = &ptr_to_mem[0]; 
				}
			return 1; // OK			
		}

	if (msg->id ==  PRIVATE_DATA)
		{
			recvfrom(sockfd, &msg->sender[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &msg->receiver[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			msg->data_lenght = ntohs(temp);
			if(msg->data_lenght >0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					recvfrom(sockfd, &ptr_to_mem[0], msg->data_lenght, 0, (struct sockaddr *) &in_address, &inlen);
					*ptr = &ptr_to_mem[0]; 
				}
			return 1; // OK			
		}

// recvfroming server messages send to client

	if (msg->id ==  REGISTER_USER_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

	if (msg->id ==  LOGIN_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

	if (msg->id ==  SHOW_CHAT_ROOMS_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			msg->data_lenght = ntohs(temp);

			if (msg->data_lenght > 0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					recvfrom(sockfd, &ptr_to_mem[0], msg->data_lenght, 0, (struct sockaddr *) &in_address, &inlen);
					*ptr = &ptr_to_mem[0]; 
					return 1;
				}
			else
				{
					return 1;
				}
		}

	if (msg->id ==  SHOW_USERS_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			recvfrom(sockfd, &temp, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			msg->data_lenght = ntohs(temp);
			if(msg->data_lenght >0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					recvfrom(sockfd, &ptr_to_mem[0], msg->data_lenght, 0, (struct sockaddr *) &in_address, &inlen);
					*ptr = &ptr_to_mem[0]; 
					return 1;
				}
			else
				{
					return 1;
				}
		}

	if (msg->id ==  JOIN_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

	if (msg->id ==  EXIT_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}

	if (msg->id ==  CREATE_CHAT_ROOM_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			return 1; // OK			
		}


	if (msg->id ==  PRIVATE_DATA_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			if (msg->error_code != SUCCESS)
				{
					recvfrom(sockfd, &msg->receiver[0], NAME_LENGHT, 0, (struct sockaddr *) &in_address, &inlen); // who was not availble
				}
			return 1; // OK
		}			

	if (msg->id ==  CHAT_DATA_RESPONSE)
		{
			recvfrom(sockfd, &msg->error_code, sizeof(int), 0, (struct sockaddr *) &in_address, &inlen);
			if (msg->error_code != SUCCESS)
				{
					sendto(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT, 0, (struct sockaddr *) &in_address, inlen); // chat room name which was not availble
				}
			return 1; // OK
		}			


	printf("message id: %d is not known!\n", msg->id); 
	return 0;
}

/*just for compilation :)*/
/*int main(void){*/

/*	printf("Hello the UDP world!\n");*/
/*}*/

