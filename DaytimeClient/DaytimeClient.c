/** 
 * DaytimeClient.c; connect to a daytime server and print the date it returns.
 * This is based on the Daytime Client found in UNP Volume 1 Third Edition (W. Richard Stevens).
 */

#include<arpa/inet.h> // inet_pton()
#include<netinet/in.h> // Define sockaddr_in.
#include<stdio.h>
#include<stdlib.h> // exit()
#include<string.h> // memset()
#include<sys/socket.h> // socket(), connect(), etc
#include<unistd.h> // read()

#define BUFFER_SIZE 64

int main(int argc, char **argv)
{
	char rxbuf[BUFFER_SIZE]; // We will read bytes from the socket into this buffer.
	int sockfd; // File Descriptor for the socket.
	int n; // This will store the number bytes read from the socket.

	/**
 	 * sockaddr_in defines three important attributes: Address Family, TCP/UDP port, and IPv4 address
 	 * in sin_family, sin_port, and sin_addr respectively.
 	 *
 	 * This particular instance is going to hold those values for the daytime server we are going to connect to.
 	 */
	struct sockaddr_in servaddr;

	/**
 	 * Make sure that exactly two arguments are give (one is the program name,
 	 * the other is the IPv4 Address of the daytime server.
 	 */
	if (argc != 2) {
		printf("Usage: DaytimeClient.exe <IPAddress>\n");
		exit(EXIT_FAILURE);
	}

	/**
     * Now the magic happens.  Call socket()!
     * Calling socket, if succesful, will return a file descriptor to perform IO on.
     * We need to pass it the the address family (AF_INET for IPv4), type (SOCK_STREAM) for TCP,
     * and protocol (0 for let the system decide) for the socket we would like to create.
     */

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("Unable to retrieve FD for socket.\n");
		exit(EXIT_FAILURE);
	}

	/**
 	 * Write zeroes to the servadr structure.
 	 */
	memset(&servaddr, 0, sizeof(servaddr));

	/**
     * Fill out the servaddr structure so we can connect to the daytime server.
     */
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13); // Converts the port number to "Network Byte Order."

	// Converts the user supplied IP Address to the network byte order and puts in the struct.
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
		printf("Could not convert %s to network byte order.\n", argv[1]);
		exit(EXIT_FAILURE);
	}		

	/**
     * Now that our socket is prepped and the server's sockaddr_in structure is prepared,
     * we can try to connect() to the server. Ufortunately for us, the connect call
     * requires an ugly last minute cast to the generic "sockaddr". This is bad for us because
     * it looks ugly, BUT it is good for the world because we can write general non-protocol specific
     * code.
     */

	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
		printf("Could not connect to server %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/**
     * After a succesful connection establishment, we are going to read whatever
     * the socket sends us into the receive buffer (rxbuf) and print it to the console.
     */
	while ( (n = read(sockfd, rxbuf, BUFFER_SIZE)) > 0 ) {
		printf("%s", rxbuf);
	}
	
	return 0;
}
