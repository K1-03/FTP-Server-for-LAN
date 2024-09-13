#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_COMMAND_LINE_LENGTH 512

int client_sock_fd;
char *err;

void download(char *cl){
	int argc;
	char *arg, *substring;
	char *filename;
	FILE *file;
	char file_bytes[1024];
	unsigned int bytes_read;

	filename = malloc(512);
	memset(filename, '\0', 512);

	argc = 0;
	strtok(cl, " ");
	
	while ((substring = strtok(NULL, " ")) != NULL){
		//Will eventually implement flags (e.x. for file compression).
		//Right now though, only command lines with one argument are valid (syntax: download [file])
		arg = substring;
		++argc;
	}

	if (argc == 1){

		strcat(filename, "repo/");
		strcat(filename, arg);
		
	    if ((file = fopen(filename, "r")) != NULL){
			while((bytes_read = fread(file_bytes, 1, 1024, file)) > 0){
				send(client_sock_fd, &file_bytes, bytes_read, 0);
				memset(file_bytes, '\0', 1024);
			}
		}
		else{
			err = "File does not exist.\n\0";
			send(client_sock_fd, err, strlen(err), 0);
		}
	}
	else if (argc == 0){   //I believe this is unreachable now, since the client checks if there are 0 args before sending the command
		/*err = "Error : No argumenets to command \"download\"\n{syntax - download [file]}\n";
		send(client_sock_fd, err, strlen(err), 0);*/
	}
	else{   //This will not be an error in the near future (currently unreachable.)
		/*err = "Error : Too many argumenets to command \"download\"\n{syntax - download [file]}\n";
		send(client_sock_fd, err, strlen(err), 0);*/
	}

	close(client_sock_fd);
	free(filename);
}

int main(){
	int sockfd;
	struct sockaddr_in addr;
	socklen_t addrlen;
	char *client_command_line;
	char *ccl_copy;
	char *client_command;
	int bytes_read;

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
		err = malloc(512);
		client_command_line = malloc(MAX_COMMAND_LINE_LENGTH);
		ccl_copy = malloc(MAX_COMMAND_LINE_LENGTH);
		bytes_read = 0;

		if (listen(sockfd, 100)){
			perror("listen");
			exit(2);
		}


		if ((client_sock_fd = accept(sockfd, (struct sockaddr*) &addr, &addrlen)) < 0){
			perror("accept");
			exit(3);
		}
		
		if ((bytes_read = recv(client_sock_fd, client_command_line, MAX_COMMAND_LINE_LENGTH, 0)) < 0){
			perror("recv");
			exit(4);
		}
		else if (bytes_read == 0){
			continue;
		}


		strcpy(ccl_copy, client_command_line);

		client_command = strtok(ccl_copy, " ");

		if (strcmp(client_command, "download") == 0){
			download(client_command_line);
		}	
		else{
			err = "Unrecognized Command\n";
			send(client_sock_fd, err, strlen(err), 0);
		}


		free(client_command_line);
		free(ccl_copy);
	}
}
