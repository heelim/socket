#include "CommunicationManager.h"
class CommunicationManager {
public:
	CommunicationManager::CommunicationManager(){
	}
	
private:
	char ip_addr[16]; 
	int port;
	int fport;
	int sock;
	struct sockaddr_in addr;
	int addrlen, readlen;	
	int fsock;
	struct sockaddr_in file_addr;

	fsock = socket(AF_INET, SOCK_STREAM, 0);
	if(fsock < 0) {
		printf("socket init error\n");
		return -1;
	}
	memset(&file_addr, 0, sizeof(file_addr));
	file_addr.sin_family = AF_INET;
	file_addr.sin_addr.s_addr = inet_addr(ip_addr);
	file_addr.sin_port = htons(9999);

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
};

