#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
// #include "CommunicationManager.h"
class CommunicationManager {
public:
	// CommunicationManager();
	// sockaddr_in* addr_init(char*, int, sockaddr_in);
	void addr_init(int, sockaddr_in*);
	int sock_init(sockaddr_in*);
	void tcp_listen(int);
	int tcp_connect(int, sockaddr_in*);
	void tcp_close(int);
private:
	// char ip_addr[16]; 
	// int port;
	// int fport;
	// int sock;
	// int fsock;
	// int addrlen, readlen;	

	// struct sockaddr_in addr;
	// struct sockaddr_in file_addr;
};
// sockaddr_in* CommunicationManager::addr_init(char* ip, int port, sockaddr_in addr) {
// 	memset(&addr, 0x00, sizeof(addr)); 
// 	addr.sin_family = AF_INET; 
// 	addr.sin_addr.s_addr = inet_addr(ip);
// 	addr.sin_port = htons(port); 

// 	return &addr;
// }
void CommunicationManager::addr_init(int port, sockaddr_in* addr) {
	memset(addr, 0x00, sizeof(*addr)); 
	(*addr).sin_family = AF_INET; 
	(*addr).sin_addr.s_addr = htonl(INADDR_ANY); 
	(*addr).sin_port = htons(port); 
}
int CommunicationManager::sock_init(sockaddr_in* addr) {
	int sock;
	if(socket(AF_INET, SOCK_STREAM, 0) < 0){ 
		perror("socket_init error"); 
		return 1; 
	}
	else {
		if(bind(sock, (struct sockaddr *)addr, sizeof(*addr)) < 0){ 
			perror("bind error"); 
			return 1; 
		} 
		return sock;
	}
}
void CommunicationManager::tcp_listen(int sock) {
	if(listen(sock, 5) < 0){ 
		perror("listen error"); 
		return; 
	} 
}
int CommunicationManager::tcp_connect(int sock, sockaddr_in* addr) {
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		printf("connection error");
		close(sock);
		return -1;
	}
}
void CommunicationManager::tcp_close(int sock) {
	close(sock);
}