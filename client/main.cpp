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

	char wbuf[BUFSIZE];
	char rbuf[BUFSIZE];

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
	while(1) {
		fgets(wbuf, BUFSIZE-1, stdin);
		readlen = strlen(wbuf);	
		wbuf[readlen-1] = '\0';
		if(!strcmp(wbuf, "q")) break;
		if(readlen < 2) continue;
		write(sock, wbuf, readlen-1);

		recvbyte = 0;
		maxbuf = BUFSIZE-1;
		do {
			recvbyte += read(sock, rbuf, maxbuf);
			maxbuf -= recvbyte;
		} while(recvbyte < readlen-1);
		rbuf[recvbyte]='\0';
		printf("Server : %s\n", rbuf);
	}
	close(sock);
	printf("connection closed\n");

	return 0;
}
