#include "CommunicationManager.h"
int CommunicationManager::tcp_listen(int host, int port, int backlog) {
    int sock;
    struct sockaddr_in servaddr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket fail");
        exit(1);
    }
    bzero((char *)&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(host);
    servaddr.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind fail");  exit(1);
    }
    listen(sock, backlog);
    return sock;
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