#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 52223

int main(int argc, char const* argv[])
{
    int sockfd;
    ssize_t buffer_size;
    struct sockaddr_in server_addr, client_addr;
    char buffer[100] = { 0 };

	// Create a TCP socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}

	// Setup the server address
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("10.0.2.15");
	server_addr.sin_port = htons(PORT);

	// Bind the socket to the address and port
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
	    0) {
		error("ERROR on binding");
	}

	// Listen for incoming connections
	listen(sockfd, 5);
	client_len = sizeof(client_addr);
	printf("Listening...\n");

	// Accept a connection from a client
	newsockfd =
	    accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (newsockfd < 0) {
		error("ERROR on accept");
	}
	printf("Connected!\n");

    buffer_size = read(new_socket, buffer, 99);

    printf("%s\n", buffer);

    // closing the connected socket
    close(newsockfd);
    close(sock_fd);

    return 0;
}

