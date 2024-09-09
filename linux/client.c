#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>

#define MAX_COMMAND_LINE_LENGTH 512

int main(int argc, char **argv){
	int sockfd;
	struct sockaddr_in addr;
	char *command_line;

	while(1){
		command_line = malloc(MAX_COMMAND_LINE_LENGTH);

		sockfd = socket(AF_INET, SOCK_STREAM, 0);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(1804);
		addr.sin_addr.s_addr = htonl(INADDR_ANY); 	

		connect(sockfd, &addr, sizeof(struct sockaddr_in));
		printf("> ");
		fgets(command_line, MAX_COMMAND_LINE_LENGTH, stdin);
		
		send(sockfd, command_line, MAX_COMMAND_LINE_LENGTH, 0);
		free(command_line);
	}
}
