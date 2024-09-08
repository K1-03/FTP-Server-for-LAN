#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/ip.h>


int main(){
	int sockfd;
	struct sockaddr_in addr;
	char *msg = malloc(10);
      
    msg = "Hello\0";	
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(19432);
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 	

	connect(sockfd, &addr, sizeof(struct sockaddr_in));

	send(sockfd, msg, 10, 0); 
}
