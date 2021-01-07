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
	//char *ip_addr = "127.0.0.1"; //localhost
	int port = 8888; //random port
	int sock, client_sock; 
	struct sockaddr_in addr, client_addr; 
	
	char rbuf[BUFSIZE]; 
	int readlen, addr_len, recv_len; 
	
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
	addr_len = sizeof(client_addr); 
	printf("waiting for clinet..\n"); 

	while((client_sock = accept(sock, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len)) > 0){ 
		printf("clinet ip : %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port)); 
		while(1) {
			readlen = read(client_sock, rbuf, sizeof(rbuf)-1);
			if(readlen == 0) break;
			rbuf[readlen]='\0';
			printf("Client(%d): %s\n", ntohs(client_addr.sin_port), rbuf);
			write(client_sock, rbuf, strlen(rbuf));
		} 
		close(client_sock); 
	} 
	close(sock); 
	return 0; 
}