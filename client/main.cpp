#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024
int main(int argc, char* argv[]) {
	char *ip_addr = "127.0.0.1"; //server ip addr
	int port = 8888; //server port num

	int sock;
	struct sockaddr_in addr;
	int addrlen, readlen, recvbyte, maxbuf;

	char rbuf[BUFSIZE];
	char wbuf[BUFSIZE];
	char buf[BUFSIZE];

	int maxfds;
	fd_set fds;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		printf("socket init error\n");
		return -1;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr);
	addr.sin_port = htons(port);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		printf("connection error");
		close(sock);
		return -1;
	}
	char str[10];
	readlen=read(sock, str, 10-1);
	str[readlen] = '\0';
	fprintf(stderr, "Server connected, you are Client(%s)\n", str);
	maxfds = sock + 1;
	FD_ZERO(&fds);
	while(1) {
		fprintf(stderr, "ME: ");
		FD_SET(0, &fds);
		FD_SET(sock, &fds);
		if (select(maxfds, &fds, NULL, NULL, NULL) < 0){
			printf("select fail");
			exit(0);
		}
		if (FD_ISSET(sock, &fds)) {
			if ((readlen = read(sock, rbuf, BUFSIZE)) > 0) {
				rbuf[readlen] = '\0';
				fprintf(stderr, "\r%s", rbuf);	
			}
		}
		if (FD_ISSET(0, &fds)) {
			if (fgets(buf, BUFSIZE, stdin)) {
				if(strlen(buf)<2) {
					//fprintf(stderr,"\r");
					continue;
				}
				if(strstr(buf, "/quit") != NULL) {
					printf("Good bye.\n");
					close(sock);
					exit(0);
				}
				if(buf[0]== '/') {
					sprintf(wbuf, "%s\n", buf);
				}
				else sprintf(wbuf, "Client(%s): %s\n", str, buf);
				if (write(sock, wbuf, strlen(wbuf)) < 0) 
					printf("Error : Write error on socket.");
			}
		}
	}
	return 0;
}
