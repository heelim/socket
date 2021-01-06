

#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
int printAddr(struct sockaddr_in *);

int main(int argc, char* argv[]) {
	char *ip_addr = "127.0.0.1"; //localhost
	int port = 8888; //random port
	int sock, client_sock; 
	struct sockaddr_in addr, client_addr; 
	char buffer[1024]; 
	int len, addr_len, recv_len; 
	if((sock = socket(AF_INET, SOCK_STREAM, 0))<0){ 
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
		printf("clinet ip : %s\n", inet_ntoa(client_addr.sin_addr)); 
		if((recv_len = read(client_sock, buffer, 1024)) < 0){ 
			perror("read "); 
			return 1; 
			break; 
		} 
		buffer[recv_len] = '\0'; 
		printf("received data : %s\n", buffer); 
		write(client_sock, buffer, strlen(buffer)); 
		close(client_sock); 
	} 
	close(sock); 
	return 0; 
}


int printAddr(struct sockaddr_in *addr) {
	int port;
	char ip_addr[18];
	port = ntohs((*addr).sin_port);
	inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), ip_addr, sizeof(struct sockaddr_in));
	printf("IP: %s, Port: %d\n", ip_addr, port);
	return 0;
}