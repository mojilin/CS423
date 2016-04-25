
#include "networking.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
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
	 close(sockfd);
     return newsockfd; 
}

int channel_write(int fd, n_state state, void * buf, int nbytes)
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


n_state channel_read(int sockfd, void * buf, int nbytes)
{
	int retval = 0;
	char temp[10];
	n_state state;
	while(read(sockfd, temp, 2) <= 0);


	state = strtol(temp, NULL, 10);
	write(sockfd, "ACK", 10);

	switch(state)
	{
		case JOB_TRANSFER:
			while(retval <= 0)
			{
				retval = read(sockfd, buf, nbytes);
			}
			printf("Message received = %s\n", (char *)buf);
			write(sockfd ,buf, nbytes);
			break;

		case STATE_TRANSFER:
			break;
			
	}

	return state;
}
