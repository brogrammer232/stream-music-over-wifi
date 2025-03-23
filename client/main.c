/*
What this code does:
1. Opens a socket and connects to the server.
2. Receives audio data from the server.
3. Plays the audio.
4. Quits when all the audio is played.
*/

// Imports.
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// Global constants.
#define PORT "7777"

// Declaring functions.
struct addrinfo *getaddressinfo();
int connect_server(struct addrinfo *res);

// Main function.
int main() {
	// Declaring variables.
	struct addrinfo *res;
	int sockfd;

	// Connecting to the server.
	res = getaddressinfo();
	sockfd = connect_server(res);	

	// Freeing memory and exiting.
	close(sockfd);
	exit(EXIT_SUCCESS);
}

// Defining functions.
struct addrinfo *getaddressinfo() {
	/* This function prepares everything needed to call getaddrinfo()
	then calls it. It returns a pointer to the linked list created
	by the function. */
	struct addrinfo hints, *res;
	int status;

	// Setting up hints.
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;

	// Getting address information.
	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	// Returning the linked list.
	return res;
}

int connect_server(struct addrinfo *res) {
	/* This function creates a socket and connects to the server. The
	server socket file descriptor is returned. */
	int sockfd, status;

	// Creating a socket.
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) {
		freeaddrinfo(res);
		perror("socket() error");
		exit(EXIT_FAILURE);
	}

	// Connecting to the server.
	printf("Connecting to the server.\n");
	status = connect(sockfd, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	if (status == -1) {
		perror("connect() error");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	printf("Successfully connected to the server.\n");

	// Returning the file descriptor.
	return sockfd;
}