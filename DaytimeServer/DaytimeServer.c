/**
 * DaytimeServer.c; source code for a program that listens on TCP port 13 
 * and returns a formatted string containing the following in accordance
 * with RFC 867: Weekday, Month Day, Year Time-Zone
 * For Example:  Tuesday, February 22, 1982 1737:43-PST
 */
// System Libraries
#include<stdio.h>
#include<string.h>
#include<time.h>

// Socket Libraries
#include<arpa/inet.h>
#include<sys/socket.h>

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

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13);

	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	listen(listenfd, 10);

	while (1) {
		connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);

		currentTime = time(NULL);
		struct tm *detailedTime = localtime(&currentTime);
		strftime(txbuf, BUFFER_SIZE, "%A, %B %d, %Y %H:%M-%Z\r\n", detailedTime);
		
		write(connfd, txbuf, BUFFER_SIZE);

		memset(txbuf, 0, BUFFER_SIZE);
		close(connfd);
	}
	/**
	time_t currentTime = time(NULL); // Get the current calendar time.

	struct tm *detailedTime = localtime(&currentTime); // Create a struct tm from the current time.

	strftime(strTime, 64, "%A, %B %d, %Y %H:%M-%Z", detailedTime); // Format the time according to RFC 867 and store it in a string.

	printf("%s", strTime);
     */	
	return 0;
}
