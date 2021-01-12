#include "CommunicationManager.h"
int CommunicationManager::tcp_listen(int ip, int port, int backlog) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket fail");
        exit(1);
    }
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(ip);
    addr.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind fail");  exit(1);
    }
    listen(sock, backlog);
    return sock;
}

int CommunicationManager::tcp_connect(char* ip_addr, int port) {
    int sock;
    sockaddr_in addr;
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
    return sock;
}
void CommunicationManager::tcp_close(int sock) {
	close(sock);
}