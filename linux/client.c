#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define MAX_COMMAND_LINE_LENGTH 512

int main(){
	int sockfd;
	struct sockaddr_in addr;
	char *command_line, *cl_copy, *command, *args, *arg;
	char *substring;
	int argc;
	char *bytes;
	int bytes_read;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(1804);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	while(1){
		command_line = malloc(MAX_COMMAND_LINE_LENGTH);
		cl_copy = malloc(MAX_COMMAND_LINE_LENGTH);
		bytes = malloc(1024);
		argc = 0;

		printf("> ");
		fgets(command_line, MAX_COMMAND_LINE_LENGTH, stdin);
		
		command_line[strlen(command_line) - 1] = '\0';  //Removing newline from buffer.

		if (strlen(command_line) == 0){
			close(sockfd);
			continue;
		}

		strcpy(cl_copy, command_line);

		command = strtok(cl_copy, " ");

		if (strcmp(command, "download") == 0){  //This will be replaced with a generic "valid command" case. All valid commands need to have their args parsed.
			while ((substring = strtok(NULL, " ")) != NULL){
			//Will eventually implement flags (e.x. for file compression).
			//Right now though, only command lines with one argument are valid (syntax: download [file])
				arg = substring;
				++argc;
			}

				if (argc == 1){
					sockfd = socket(AF_INET, SOCK_STREAM, 0);
					connect(sockfd, &addr, sizeof(struct sockaddr_in));  //This is absolutely not a change that will stick around. But socket creation should
																			//not occur until after a command is validated and this is the only valid path for
																			//a command currently.

					send(sockfd, command_line, MAX_COMMAND_LINE_LENGTH, 0);
					while((bytes_read = recv(sockfd, bytes, 1024, 0)) > 0){
						printf("%s", bytes);
						memset(bytes, '\0', 1024);
					}
				}
				else if (argc == 0){ 
					printf("Error : No argumenets to command \"download\"\n{syntax - download [file]}\n");
					//close(sockfd);
				}
				else{
					printf("Error : Too many argumenets to command \"download\"\n{syntax - download [file]}\n");
					//close(sockfd);
				}
		}
		else{
			printf("Unrecognized command \"%s\"\n", command);
			//close(sockfd);
		}
		
		
		free(command_line);
		free(cl_copy);
	}
}
