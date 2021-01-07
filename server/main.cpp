#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

#define BUFSIZE 1024
int main(int argc, char* argv[]) {
	//char *ip_addr = "127.0.0.1"; //localhost
	int port = 8888; //random port
	int sock, client_sock; 
	struct sockaddr_in addr, client_addr; 
	
	char rbuf[BUFSIZE]; 
	int readlen, addr_len, recv_len; 
	
	int maxfd = 0;
	fd_set fds, rfds;
	int res, i;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){ 
		perror("socket "); 
		return 1; 
	} 
	memset(&addr, 0x00, sizeof(addr)); 
	addr.sin_family = AF_INET; 
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	addr.sin_port = htons(port); 

	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){ 
		perror("bind error"); 
		return 1; 
	} 
	if(listen(sock, 5) < 0){ 
		perror("listen error"); 
		return 1; 
	} 

	FD_ZERO(&fds);
	FD_SET(sock, &fds);
	maxfd = sock;

	addr_len = sizeof(client_addr); 
	printf("waiting for client..\n"); 

	while(1){
		rfds=fds;
		printf("monitering\n");
		if(select(maxfd+1, &rfds, 0, 0, NULL) < 0) break;
		for(i=0; i<maxfd+1; i++) {
			if(FD_ISSET(i, &rfds)) {
				if(i==sock){
					client_sock = accept(sock, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len); 
					if(client_sock == -1) {
						printf("accept error\n");
						continue;
					}
					printf("client connected\n");
					printf("clinet ip : %s: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
					FD_SET(client_sock, &fds);
					if(maxfd<client_sock) maxfd=client_sock;
				}
				else {
					readlen = read(i, rbuf, sizeof(rbuf)-1);
					if(readlen == 0) {
						printf("client disconnected\n");
						FD_CLR(i, &fds);
						close(i);
						continue;
					};
					rbuf[readlen]='\0';
					printf("Client(%d): %s\n", i-3, rbuf);					
					write(i, rbuf, strlen(rbuf));
				}
			}
		}
	} 
	close(sock); 
	return 0;
}