#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

const char address[] = "172.22.146.190";
#define PORT 4200

int state = 0;
int join_channel(const char * address, int port);

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd;
	char buffer[100];
	sockfd = join_channel(address, PORT);
	
	while(1)
	{
		while(1)
		{
			int n = read(sockfd, buffer, 10);
			if(n > 0)
				break;
		}

		state = strtol(buffer, NULL, 10);
		
		puts("State received");

		write(sockfd, "ACK", 10);

		switch(state)
		{
			case 1:
				while(read(sockfd, buffer, 100) <= 0);
				printf("Message received: %s\n", buffer);
				write(sockfd ,"ACK", 10);
				break;
		}
	
	}
	close(sockfd);
    return 0;
}

int join_channel(const char * address, int port)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(address);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
	printf("Creating connecttion to address: %s at port %d\n", address, port);
	puts("Connecting to server...");
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	puts("Connection established!");

    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
	puts("Received ACK from server");
    return sockfd;
}
