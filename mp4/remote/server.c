/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 4200

int create_channel(int port);
int channel_write(int fd, void * buf, int nbytes);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd;
	 char buffer[100];
	 sockfd = create_channel(PORT);

	 while(1)
	 {
		printf("Enter message:");
		scanf("%s", buffer);
		channel_write(sockfd, buffer, 100);
	 }

     return 0; 
}


/*
 * create_channel
 * Creates a new socket with the given port
 * Inputs: port -- Port number to bind the socket to
 * Return Value: The fd of the socket connected to the client
 * 				 Does not return until a connection has been
 * 				established by a client
 */
int create_channel(int port)
{

     int sockfd, newsockfd, portno;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = port;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

	 printf("Created socket on port %d\n", port);
     listen(sockfd,5);
	 puts("Waiting for client to connect.... ");
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     puts("Client to server connection established");
     n = write(newsockfd,"Client connection ACK",25);
     if (n < 0) error("ERROR writing to socket");
     return newsockfd; 
}

int channel_write(int fd, void * buf, int nbytes)
{
	char buffer[10];
	write(fd, "1", 10);

	while(read(fd, buffer, 10) <= 0);
	printf("Buffer received = %s\n", buffer);
	write(fd, buf, nbytes);

	while(read(fd, buffer, 10) <= 0);
	
	printf("Buffer received = %s\n", buffer);
	
	return 0;
}
