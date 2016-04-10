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

int send_universal_india_message( struct india_universal_message msg, int sockfd, char* ptr )
{

// send msg ID

	int retValue;
	int id = htons(msg.id);
	retValue = write(sockfd, &id, sizeof(id));

	if(retValue < 1)
		{
			printf("India_messages_protocol: write retValue: %d\n", retValue);
			return -1;
		}


// handling of client messages sending to server

	if (msg.id ==  REGISTER_USER)
		{
      
			write(sockfd, &msg.proposed_nickname[0], NAME_LENGHT);
			write(sockfd, &msg.password[0], PW_LENGHT);
			return 1; // OK
		}

	if (msg.id ==  LOGIN)
		{
			write(sockfd, &msg.sender[0], NAME_LENGHT);
			write(sockfd, &msg.password[0], PW_LENGHT);
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
			write(sockfd, &msg.sender[0], NAME_LENGHT);
			write(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			return 1; // OK
		}

	if (msg.id ==  EXIT)
		{
			write(sockfd, &msg.sender[0], NAME_LENGHT);
			write(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			return 1; // OK
		}

	if (msg.id ==  CREATE_CHAT_ROOM)
		{
			write(sockfd, &msg.sender[0], NAME_LENGHT);
			write(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			return 1; // OK
		}

// messages used in both direction

	int temp;


	if (msg.id ==  CHAT_ROOM_DATA)
		{
			temp = htons(msg.data_lenght);
			write(sockfd, &msg.sender[0], NAME_LENGHT);
			write(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			write(sockfd, &temp, sizeof(int));
			write(sockfd, &ptr[0], msg.data_lenght);
			return 1; // OK
		}

	if (msg.id ==  PRIVATE_DATA)
		{
			temp = htons(msg.data_lenght);
			write(sockfd, &msg.error_code, sizeof(int));
			write(sockfd, &msg.sender[0], NAME_LENGHT);
			write(sockfd, &msg.receiver[0], NAME_LENGHT);
			write(sockfd, &temp, sizeof(int));
			write(sockfd, (const void *) &ptr[0], msg.data_lenght);
			return 1; // OK
		}

// handling of server messages sending to client

	if (msg.id ==  REGISTER_USER_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			return 1; // OK
		}

	if (msg.id ==  LOGIN_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			return 1; // OK
		}

	if (msg.id ==  SHOW_CHAT_ROOMS_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			if (msg.data_lenght == 0)
				{
					// no chat rooms available
					return 1; // OK
				}
			else
				{
					temp = htons(msg.data_lenght);
					write(sockfd, &temp, sizeof(int));
					write(sockfd, (const void *) &ptr[0], msg.data_lenght);
					return 1;
				}
		}

	if (msg.id ==  SHOW_USERS_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			if (msg.data_lenght == 0)
				{
					// no users available
					return 1; // OK
				}
			else
				{
					temp = htons(msg.data_lenght);
					write(sockfd, &temp, sizeof(int));
					write(sockfd, (const void *) &ptr[0], msg.data_lenght);
					return 1;
				}

		}

	if (msg.id ==  JOIN_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			return 1; // OK
		}

	if (msg.id ==  EXIT_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			return 1; // OK
		}

	if (msg.id ==  CREATE_CHAT_ROOM_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			return 1; // OK
		}

	if (msg.id ==  PRIVATE_DATA_RESPONSE)
		{
			write(sockfd, &msg.error_code, sizeof(int));
			if (msg.error_code != SUCCESS)
				{
					write(sockfd, &msg.receiver[0], NAME_LENGHT); // who was not availble
				}
			return 1; // OK
		}

	if (msg.id ==  CHAT_DATA_RESPONSE)
		{
			write(sockfd, &msg.error_code, 1);
			if (msg.error_code != SUCCESS)
				{
					write(sockfd, &msg.chat_room_name[0], CHAT_ROOM_NAME_LENGHT); // chat room name which was not availble
				}
			return 1; // OK
		}


	// message not known
	printf("message id: %d is not known!\n", msg.id);
	return 0; //
}


// READING FUNCTION

/* create empty message struct, function will add relevant fields to it*/
/* give ptr for data buffer */

int read_universal_india_message( struct india_universal_message* msg,  int sockfd, char **ptr )
{

// read msg ID

	int id = 0;
	int retValue = 0;
	retValue = read(sockfd, &id, sizeof(id));

	if(retValue <1)
		{
//			printf("India_messages_protocol: read retValue: %d\n", retValue);
			perror("Read Error:");
			return -1;
		}

	msg->id = ntohs(id);
	char *ptr_to_mem;
	int temp;

// handling of client messages reading in server

	if (msg->id ==  REGISTER_USER)
		{
			read(sockfd, &msg->proposed_nickname[0], NAME_LENGHT);
			read(sockfd, &msg->password[0], PW_LENGHT);

			return 1; // OK
		}

	if (msg->id ==  LOGIN)
		{
			read(sockfd, &msg->sender[0], NAME_LENGHT);
			read(sockfd, &msg->password[0], PW_LENGHT);
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
			read(sockfd, &msg->sender[0], NAME_LENGHT);
			read(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			return 1; // OK
		}

	if (msg->id ==  EXIT)
		{
			read(sockfd, &msg->sender[0], NAME_LENGHT);
			read(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			return 1; // OK
		}

	if (msg->id ==  CREATE_CHAT_ROOM)
		{
			read(sockfd, &msg->sender[0], NAME_LENGHT);
			read(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			return 1; // OK
		}

// messages used in both direction



	if (msg->id ==  CHAT_ROOM_DATA)
		{
			read(sockfd, &msg->sender[0], NAME_LENGHT);
			read(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT);
			read(sockfd, &temp, sizeof(int));
			msg->data_lenght = ntohs(temp);
			if(msg->data_lenght >0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					read(sockfd, &ptr_to_mem[0], msg->data_lenght);
					*ptr = &ptr_to_mem[0];
				}
			return 1; // OK
		}

	if (msg->id ==  PRIVATE_DATA)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			read(sockfd, &msg->sender[0], NAME_LENGHT);
			read(sockfd, &msg->receiver[0], NAME_LENGHT);
			read(sockfd, &temp, sizeof(int));
			msg->data_lenght = ntohs(temp);
			if(msg->data_lenght >0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					read(sockfd, &ptr_to_mem[0], msg->data_lenght);
					*ptr = &ptr_to_mem[0];
				}
			return 1; // OK
		}

// reading server messages send to client

	if (msg->id ==  REGISTER_USER_RESPONSE)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			return 1; // OK
		}

	if (msg->id ==  LOGIN_RESPONSE)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			return 1; // OK
		}

	if (msg->id ==  SHOW_CHAT_ROOMS_RESPONSE)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			read(sockfd, &temp, sizeof(int));
			msg->data_lenght = ntohs(temp);

			if (msg->data_lenght > 0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					read(sockfd, &ptr_to_mem[0], msg->data_lenght);
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
			read(sockfd, &msg->error_code, sizeof(int));
			read(sockfd, &temp, sizeof(int));
			msg->data_lenght = ntohs(temp);
			if(msg->data_lenght >0)
				{
					ptr_to_mem = (char *) malloc(msg->data_lenght);
					read(sockfd, &ptr_to_mem[0], msg->data_lenght);
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
			read(sockfd, &msg->error_code, sizeof(int));
			return 1; // OK
		}

	if (msg->id ==  EXIT_RESPONSE)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			return 1; // OK
		}

	if (msg->id ==  CREATE_CHAT_ROOM_RESPONSE)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			return 1; // OK
		}


	if (msg->id ==  PRIVATE_DATA_RESPONSE)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			if (msg->error_code != SUCCESS)
				{
					read(sockfd, &msg->receiver[0], NAME_LENGHT); // who was not availble
				}
			return 1; // OK
		}

	if (msg->id ==  CHAT_DATA_RESPONSE)
		{
			read(sockfd, &msg->error_code, sizeof(int));
			if (msg->error_code != SUCCESS)
				{
					write(sockfd, &msg->chat_room_name[0], CHAT_ROOM_NAME_LENGHT); // chat room name which was not availble
				}
			return 1; // OK
		}


	printf("message id: %d is not known!\n", msg->id);
	return 0;
}
