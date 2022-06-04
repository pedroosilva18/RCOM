//#include "../include/download.h"
#include "download.h"
#include "macros.h"

int main(int argc, char *argv[])
{	

    requestedData data;

    if(argc != 2){
        printf("Invalid arguments. Example:: ftp://[<user>:<password>@]<host>/<url-path>\n");
        return -1;
    }

    parse_input(argv[1], &data);
	print_data_struct(&data);

    struct hostent *h = getIP(data); 
	char *ip = inet_ntoa(*((struct in_addr *)h->h_addr));

	printf("Requesting socket to port 21\n");
	int socket_request = socket_config(ip, SERVER_PORT);
	printf("Connected\n");

	if (read_reply(socket_request) != 0)
	{
		printf("ERROR IN COMMANDS\n");
		close(socket_request);
		return -1;
	}

  	// login
	char command[MAX_STRING_SIZE*2];
	sprintf(command, "user %s", data.user);
	printf ("COMMAND: %s\n", command);
	send_command(socket_request, command);
	if (read_reply(socket_request) != 0)
	{
		printf("ERROR IN COMMANDS\n");
		close(socket_request);
		return -1;
	}
	
	sprintf(command, "%s", data.password);
	printf ("COMMAND: %s\n", command);
	send_command(socket_request, command);
	if (read_reply(socket_request) != 0)
	{
		printf("ERROR IN COMMANDS\n");
		close(socket_request);
		return -1;
	}

	send_command(socket_request, "pasv");
	

	/*
	char buf[1000000];
	printf("\n\n");
	while(1)
	{
	 	ssize_t a = read(socket_request, &buf, 1000);
		printf("\n%ld -- %s\n", a,buf); 
	}

*/
  	
	
	/*char buf[2048] = "12345";
	int buf_size;
	//buf_size = read_socket_reply(socket_request, buf);
	/*buf_size = send(socket_request, buf, strlen(buf), 0);
	printf("Bytes escritos %d\n", buf_size);*/
    

	return 0;
}

int socket_config (char *ip, int port)
{
	int	sockfd;
	struct sockaddr_in server_addr;
	/*char buf[] = "Mensagem de teste na travessia da pilha TCP/IP\n";  
	int	bytes;*/
	
	/*server address handling*/
	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);	/*32 bit Internet address network byte ordered*/
	server_addr.sin_port = htons(port);		/*server TCP port must be network byte ordered */
    
	/*open an TCP socket*/
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0) 
	{
    	perror("socket()");
		exit(0);
    }
	/*connect to the server*/
    	if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
    	perror("connect()");
		exit(0);
		}
    	/*send a string to the server*/
	

	/*close(sockfd);
	exit(0);*/


	return sockfd;
}
int send_command(int socketfd, char *command)
{
	printf("/**** Sending Commands ****/\n");
	
	int bytes_sent = send(socketfd, command, strlen(command), 0);
	//printf("OH CUM CRL\n");
	if (bytes_sent < 0)
	{
		printf("Error Sending Command\n");
		return -1;
	}
	
	printf("Command sent: %s with %d bytes\n", command, bytes_sent);
	return 0;
}

int read_reply(int socketfd)
{
	char *buf = malloc(MAX_STRING_SIZE);
	ssize_t random = 0;
	FILE *file = fdopen(socketfd, "r");

	//printf("LEL\n");
	while (getline(&buf, &random, file) != -1)
	{
		printf("< %s", buf);
		//printf("LET ME SEE\n");
		if (buf[3] == ' ')
			break;
		//printf("IM KILLING U BITCH\n");
		if (buf[0] == '4' || buf[0] == '5')
		{
			printf("ERROR READING RESPONSE\n");
			close(socketfd);
			return -1;
		}
	}
	return 0;
}

struct hostent *getIP(requestedData data)
{
    struct hostent *h;
    if ((h=gethostbyname(data.host)) == NULL) 
	{  
        herror("gethostbyname");
        return NULL;
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n",inet_ntoa(*((struct in_addr *)h->h_addr)));

    return h;
}
