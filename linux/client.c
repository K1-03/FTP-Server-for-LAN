#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define MAX_COMMAND_LINE_LENGTH 512

int main(int argc, char **argv){
	int sockfd;
	struct sockaddr_in addr;
	char *command_line, *cl_copy, *command;
	char *bytes;
	int bytes_read;

	while(1){
		command_line = malloc(MAX_COMMAND_LINE_LENGTH);
		cl_copy = malloc(MAX_COMMAND_LINE_LENGTH);
		bytes = malloc(1024);

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(1804);
		addr.sin_addr.s_addr = htonl(INADDR_ANY); 	

		connect(sockfd, &addr, sizeof(struct sockaddr_in));
		printf("> ");
		fgets(command_line, MAX_COMMAND_LINE_LENGTH, stdin);
		
		command_line[strlen(command_line) - 1] = '\0';  //Removing newline from buffer.

		if (strlen(command_line) == 0){
			close(sockfd);
			continue;
		}

		strcpy(cl_copy, command_line);

		command = strtok(cl_copy, " ");

		if (strcmp(command, "download") == 0){
			printf("%s\n", command_line);
			send(sockfd, command_line, MAX_COMMAND_LINE_LENGTH, 0);
			while((bytes_read = recv(sockfd, bytes, 1024, 0)) > 0){
				printf("%s", bytes);
				memset(bytes, '\0', 1024);
			}
		}
		else{
			printf("Unrecognized command \"%s\"\n", command);
			close(sockfd);
		}
		
		
		free(command_line);
		free(cl_copy);
	}
}
