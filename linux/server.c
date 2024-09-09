#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_COMMAND_LINE_LENGTH 512

void download(char *cl){
	int argc;
	char *arg, *substring;

	argc = 0;
	strtok(cl, " ");
	
	while ((substring = strtok(NULL, " ")) != NULL){
		//Will eventually implement flags (e.x. for file compression).
		//Right now though, only command lines with one argument are valid (syntax: download [file])
		arg = substring;
		++argc;
	}

	if (argc == 1){
		//Attempt to open file in repo, and then send its contents back to client.
	}
	else{
		printf("Error : Too many argumenets to command \"download\"\n{syntax - download [file]}\n");
	}

}

int main(){
	int sockfd;
	struct sockaddr_in addr;
	socklen_t addrlen;
	int client_sock_fd;
	char *client_command_line;
	char *ccl_copy;
	char *client_command;

	addrlen = sizeof(struct sockaddr_in);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1804);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	if (bind(sockfd, &addr, sizeof(struct sockaddr_in)) < 0){ //bind returns 0 (false) on success
		perror("bind");
		exit(1);
	}

	
	while(1){
		client_command_line = malloc(MAX_COMMAND_LINE_LENGTH);
		ccl_copy = malloc(MAX_COMMAND_LINE_LENGTH);

		if (listen(sockfd, 100)){
			perror("listen");
			exit(2);
		}


		if ((client_sock_fd = accept(sockfd, (struct sockaddr*) &addr, &addrlen)) < 0){
			perror("accept");
			exit(3);
		}
		
		if (recv(client_sock_fd, client_command_line, MAX_COMMAND_LINE_LENGTH, 0) < 0){
			perror("recv");
			exit(4);
		}

		strcpy(ccl_copy, client_command_line);

		client_command = strtok(ccl_copy, " ");

		if (strcmp(client_command, "download") == 0){
			download(client_command_line);
		}


		free(client_command_line);
		free(ccl_copy);
	}
}
