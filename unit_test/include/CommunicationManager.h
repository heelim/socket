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
	int tcp_listen(int, int, int);
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