#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/types.h>

#define BUF_SIZE 8
#define ECHO_PORT 7

int main(int argc, char **argv)
{
	char txbuf[BUF_SIZE];
	char rxbuf[BUF_SIZE];
	memset(rxbuf, 0, BUF_SIZE);
	memset(txbuf, 0, BUF_SIZE);

	int serverfd;
	if ( (serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("ERROR - Could not retrieve file descriptor for socket().\n");
		exit(EXIT_FAILURE);
	}	

	
	struct sockaddr_in serverAddr;

	memset(&serverAddr, 0, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(ECHO_PORT);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)	{
		printf("ERROR - Unable to bind to socket.\n");
		exit(EXIT_FAILURE);
	}

	if (listen(serverfd, 10) < 0) {
		printf("ERROR - Unable to listen().\n");
		exit(EXIT_FAILURE);
	}

	int clientfd;
	int clientLen;
	int r = 0;
	struct sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	clientLen = sizeof(clientAddr);

	while(1) {
		clientfd = accept(serverfd, (struct sockaddr *) &clientAddr, &clientLen);

		char clientAddrStr[INET_ADDRSTRLEN];

		inet_ntop(AF_INET, &(clientAddr.sin_addr), clientAddrStr, sizeof(clientAddr));
		
		printf("Connection from client %s\n", clientAddrStr);

		while ( (r = read(clientfd, rxbuf, BUF_SIZE)) > 0) {

			int i = 0;	
			while (rxbuf[i] != '\0' && rxbuf[i] != '\n' && i < (BUF_SIZE - 1)) {
				if (rxbuf[i] >= ' ' && rxbuf[i] <= '~') {
					txbuf[i] = rxbuf[i];
					i++;
				} else {
					continue;	
					i++;
				}
			}
			txbuf[i] = '\0';

			write(clientfd, txbuf, BUF_SIZE);
			printf("Echoed %s to client %s\n", txbuf, clientAddrStr);

			memset(&txbuf, 0, BUF_SIZE);
			memset(&rxbuf, 0, BUF_SIZE);
		}
		printf("Client %s disconnected\n", clientAddrStr);
	}		

	close(serverfd);

	return 0;
}
