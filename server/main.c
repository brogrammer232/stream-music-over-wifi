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


// Global constants.
#define PORT "7777"


// Main function.
int main() {
	// Declaring variables.
	struct addrinfo hints, *res;
	int status, sockfd;

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
		exit(EXIT_FAILURE);
	}


	// Freeing up memory


	return 0;
}