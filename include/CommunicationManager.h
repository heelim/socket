#include "common.h"
 
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

