/*
This file contains server code. It will run on the phone.
Compile command:
	gcc main.c -o main -lsndfile # For running on the computer.

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
#include <unistd.h>
#include <sndfile.h>

// Global constants.
#define PORT "7777"
#define AUDIO_FILE "song.wav"

// Declaring functions (alphabetically arranged).
void connect_client(int sockfd);
int create_socket(struct addrinfo *res);
struct addrinfo *getaddressinfo();

// Main function.
int main() {
	// Declaring variables.
	struct sockaddr_in client_addr;
	SNDFILE *file;
	SF_INFO sf_info;
	int status, client_sockfd;
	socklen_t client_addr_size;

	struct addrinfo *res = getaddressinfo();
	int sockfd = create_socket(res);
	connect_client(sockfd);

	// Opening the audio file.
	file = sf_open(AUDIO_FILE, SFM_READ, &sf_info);

	// Freeing up memory and exiting.
	close(sockfd);
	close(client_sockfd);
	exit(EXIT_SUCCESS);
}

// Functions definition.
void connect_client(int sockfd) {
	/* This function connects to the client. */
	struct sockaddr_in client_addr;
	int client_sockfd, status;
	socklen_t client_addr_size;

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
}


int create_socket(struct addrinfo *res) {
	/* This function creates and binds a socket using information in
	the linked list argument. The file descriptor of the created socket
	is returned. */
	int sockfd, status;

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
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// Returning the file descriptor.
	return sockfd;
}


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
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

	// Getting the linked list.
	status = getaddrinfo(NULL, PORT, &hints, &res);
	if (status != 0) {
		fprintf(stderr, "getaddrinfo() Error: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

	// Returning the linked list.
	return res;
}