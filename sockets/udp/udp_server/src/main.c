#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512
#define PORT   1234

void die(char *s){
	perror(s);
	exit(1);
}

int main(void) {
	printf("UDP Server\n");

	struct sockaddr_in si_me, si_other;
	char buf[BUFLEN];
	char buf_out[1024];
	int socketfd;
	int recv_len;
	int send_len;
	int slen = sizeof(si_other);

	//Criar UDP Socket
	socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socketfd == -1){
		die("Couldn't open socket");
	}

	// Limpo a struct antes de usar
	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(socketfd, (struct sockaddr*)&si_me, sizeof(si_me)) == -1){
		die("Couldn't bind to port!");
	}

	while(1){
		printf("Waiting for data...\n");
		fflush(stdout);

		recv_len = recvfrom(socketfd, buf, 
							BUFLEN, 0, (struct sockaddr*)&si_other, 
							&slen);
		if(recv_len == -1){
			die("Error receiving packet!");
		} else {
			buf[recv_len] = '\0'; 
		}
	
		printf("Received packet from %s:%d\n", 
				inet_ntoa(si_other.sin_addr), 
				ntohs(si_other.sin_port));
		
		printf("Data : %s\n", buf);
		sprintf(buf_out, "Hello %s", buf);

		send_len = sendto(socketfd, buf_out, strlen(buf_out), 0,
						  (struct sockaddr*) &si_other, sizeof(si_other));
		
		if(send_len == -1){
			die("Error sending response!\n");
		}
		
	}

	close(socketfd);

	return 0;
}