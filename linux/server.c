#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>

int main(){
	int sockfd;
	struct sockaddr_in addr;
	socklen_t addrlen;
	int client_sock_fd;

	addrlen = sizeof(struct sockaddr_in);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(19432);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 

	if (bind(sockfd, &addr, sizeof(struct sockaddr_in)) < 0){ //bind returns 0 (false) on success
		perror("bind");
		exit(1);
	}

	
	while(1){
		if (listen(sockfd, 100)){
			perror("listen");
			exit(2);
		}


		char *test = malloc(10);

		if ((client_sock_fd = accept(sockfd, (struct sockaddr*) &addr, &addrlen)) < 0){
			perror("accept");
			exit(3);
		}
		
		if (recv(client_sock_fd, test, 10, 0) < 0){
			perror("recv");
			exit(4);
		}

		
		printf("%s\n", test);
	}
}
