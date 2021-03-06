/******
INDIA MESSAGE HEADER
******/
# include "india_messages_protocol.h"


int main(void)
{


	struct sockaddr_in servaddr;
	char *address2 = "127.0.0.1";
	int sockfd;

	//memset(&servaddr, 0, sizeof(servaddr));

	int port = 5555;
	int y, n =0;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(5555);

    	if (inet_pton(AF_INET, address2, &servaddr.sin_addr) <= 0)
	{
        	fprintf(stderr, "inet_pton error for %s\n", address2);
        	return -1;
    	}

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			printf("server socket error\n");
        		return -1;
		}

		/*  Connect to 'master' server */

		if (  n = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) <0)
			{
				perror("connect error");
        			return -1;
    			}


	printf("Testing india messages\n");

	struct india_universal_message msg;

	char buffer[80];

	printf("sending register\n");

	msg.id = REGISTER_USER;
	strcpy(msg.proposed_nickname, "jukka");
	strcpy(msg.password, "1234");
	msg.data_lenght = 0;
	int x;

	printf("Name: %s\n",msg.proposed_nickname);
	printf("Name: %s\n",msg.password);

	x = send_universal_india_message(msg, sockfd, &buffer[0]);

	printf("sending login\n");

	msg.id = LOGIN;
	strcpy(msg.sender, "jukka");
	strcpy(msg.password, "1234");
	msg.data_lenght = 0;

	printf("Name: %s\n",msg.sender);
	printf("Name: %s\n",msg.password);

	x = send_universal_india_message(msg, sockfd, &buffer[0]);

	msg.id = CHAT_ROOM_DATA;
	strcpy(msg.sender, "jukka");
	strcpy(msg.chat_room_name, "Keskustelua chatista");
	strcpy(&buffer[0], "It is very nice day Today");
	msg.data_lenght = strlen("It is very nice day Today");
	printf("message id = %d\n",msg.id);
	printf("Name: %s\n",msg.sender);
	printf("Chatroom: %s\n",msg.chat_room_name);
	printf("Text: %s\n",&buffer[0]);


	x = send_universal_india_message(msg, sockfd, &buffer[0]);



close(sockfd);

}
