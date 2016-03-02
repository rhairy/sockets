/**
 * DaytimeServer.c; source code for a program that listens on TCP port 13 
 * and returns a formatted string containing the following in accordance
 * with RFC 867: Weekday, Month Day, Year Time-Zone
 * For Example:  Tuesday, February 22, 1982 1737:43-PST
 */
// System Libraries
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// Socket Libraries
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFFER_SIZE 64
#define TIME_SIZE 64

int main()
{
	char txbuf[BUFFER_SIZE]; // Buffer to write to the socket.
	memset(txbuf, 0, BUFFER_SIZE);

	int connfd;
	int listenfd;

	struct sockaddr_in servaddr;

	time_t currentTime; // Will store the current calendar time.

	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("Unable to create socket.\n");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13);

	if ( (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) ) {
		printf("Unable to bind socket.\n");
		exit(EXIT_FAILURE);
	}

	if (listen(listenfd, 10)) {
		printf("Unable to listen.\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		struct sockaddr_in cliaddr;
		socklen_t addrSize = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &addrSize);

		char *clientIP = inet_ntoa(cliaddr.sin_addr);

		printf("New connection from: %s\n", clientIP); 
		currentTime = time(NULL);
		struct tm *detailedTime = localtime(&currentTime);
		strftime(txbuf, BUFFER_SIZE, "%A, %B %d, %Y %H:%M-%Z\r\n", detailedTime);
		

 		write(connfd, txbuf, BUFFER_SIZE); 
		printf("Wrote %s to %s\n", txbuf, clientIP);
		

		printf("Closing connection to %s\n", clientIP);
		memset(txbuf, 0, BUFFER_SIZE);
		close(connfd);
	}
	return 0;
}
