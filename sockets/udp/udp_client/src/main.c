#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT   1234

void die(char *s){
	perror(s);
	exit(1);
}

int main(void) {
	printf("UDP Client\n");

	struct sockaddr_in si_other;
	int socketfd;
	int recv_len;
	int slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];

	socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( socketfd == -1){
		die("Couldn't open socket");
	}

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	
	if(inet_aton(SERVER, &si_other.sin_addr) == 0) {
		die("inet_aton() failed\n");
	}

	while(1){
		printf("Enter message: ");
		if(fgets(message, BUFLEN, stdin) == NULL){
			die("Error reading input.");
		};

		if(strcmp(message, "quit") == 0){
			break;
		} else {
			printf("You typed: %s\n", message);
		}
		
		if(sendto(socketfd, message, strlen(message), 0, 
		          (struct sockaddr *)&si_other, sizeof(si_other)) == -1 ){

			die("Couldn't send message!");
		}

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		if (setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, 
						&timeout, sizeof(timeout)) == -1){
			die("Couldn't set timeout!");
		}

		memset(buf, '\0', BUFLEN);
		recv_len = recvfrom(socketfd, buf, 
							BUFLEN, 0, (struct sockaddr*)&si_other, 
							&slen);
		if(recv_len == -1){
			perror("Receive timeout!");
			//die("Error receiving packet!");
		} else {
			printf("Reply: %s\n", (char *)buf);
		}
	}

	close(socketfd);

	return 0;
}