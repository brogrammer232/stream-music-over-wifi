/*
This file contains server code. It will run on the phone.
Compile command:
	gcc main.c -o main # For running on the computer.

What this code does:
1. Open a socket and connect to the client.
2. Open the music file and send raw PCM to the client.
3. Quit when all the data is sent.
*/

// Imports.
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// Global constants.
#define PORT "7777"


// Main function.
int main() {
	// Declaring variables.
	struct addrinfo hints, *res;
	struct sockaddr_in client_addr;
	int status, sockfd, client_sockfd;
	socklen_t client_addr_size;

	// Setting up hints.
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	// Getting the linked list.
	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo() Error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	// Creating a socket.
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) {
		perror("socket() error");
		freeaddrinfo(res);
		exit(EXIT_FAILURE);
	}

	// Binding to the local IP.
	status = bind(sockfd, res->ai_addr, res->ai_addrlen);
	freeaddrinfo(res);
	if (status == -1) {
		perror("bind() error");
		exit(EXIT_FAILURE);
	}

	// Waiting for the client to connect.
	printf("Waiting for the client to connect.\n");
	status = listen(sockfd, 1);
	if (status == -1) {
		perror("listen() error");
		exit(EXIT_FAILURE);
	}

	// Accepting the connection.
	client_addr_size = sizeof(client_addr);
	client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr,
		&client_addr_size);
	if (client_sockfd == -1) {
		perror("accept() error");
		exit(EXIT_FAILURE);
	}
	printf("Successfully connected to the client.\n");


	// Freeing up memory


	exit(EXIT_SUCCESS);
}