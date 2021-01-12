#include "common.h"
 
// #include "CommunicationManager.h"
class CommunicationManager {
public:
	// CommunicationManager();
	int tcp_listen(int, int, int);
	int tcp_connect(char*, int);
	void tcp_close(int);
private:
	// struct sockaddr_in addr;
	// char ip_addr[16]; 
	// int port;
	// int sock;
	// int addrlen;	
	
};

