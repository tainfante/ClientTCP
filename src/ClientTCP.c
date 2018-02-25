/*
 ============================================================================
 Name        : ClientTCP.c
 Author      : Agata Dul
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <assert.h>


int main(int argc, char** argv) {
	system("clear");
	if(argc != 3) {
		printf("Invalid number of arguments, please declare IP address and port number/n");
		exit(1);
	}

	int	master_socket;
	struct sockaddr_in server_addr;
	socklen_t server_addr_length;
	struct hostent *server;
	char *buffer_in;
	char *buffer_out;

	int buffer_size=256;




	if((master_socket = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("Unable to create socket");
	}
	else{
		printf("Socket created, number: %d\n", master_socket);
	}


	if((server=gethostbyname(argv[1]))== NULL) {
		printf("Invalid IP address, check again/n");
		exit(1);
	}
	server_addr_length=sizeof(server_addr);

	bzero((char *) &server_addr, server_addr_length);

	bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));

	if (connect(master_socket,(struct sockaddr *) &server_addr, server_addr_length) < 0) {
		perror("Unable to connect");
		exit(1);
	}
	else{
		printf("Connected\n");
	}

	while(1) {

		puts("NEW OPERATION");
		printf("Enter the wanted operation (available: sum, dif, sortup, sortdown)\n ");
		buffer_out = (char *) malloc(0);
		fgets(buffer_out,buffer_size,stdin);
		if((write(master_socket,buffer_out,buffer_size)) <= 0) {
			perror("Unable to transmit data");
			exit(1);
		}
		free(buffer_out);
		buffer_in = (char *) malloc(4096);
		if((read(master_socket,buffer_in,4096)) < 0) {
			perror("Unable to receive result");
			exit(1);
		}

		printf("The result of your operation is: %s\n END OF OPERATION\n\n",buffer_in);
		free(buffer_in);



	}

	return 0;
}

