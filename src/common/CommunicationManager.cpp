#include "CommunicationManager.h"
int CommunicationManager::tcp_listen(int ip, int port, int backlog) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket fail\n");
        exit(1);
    }
    bzero((char *)&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(ip);
    addr.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind fail\n");  exit(1);
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
        printf("connection error\n");
        close(sock);
        return -1;
    }
    return sock;
}
void CommunicationManager::tcp_close(int sock) {
	close(sock);
}

void CommunicationManager::tcp_send_msg(int sock, char* msg) {
    if (send(sock, msg, strlen(msg), 0) < 0) 
        printf("Error : Write error on socket.\n");
}
void CommunicationManager::tcp_recv_msg(int sock, char* msg) {
    int readlen;
    readlen = recv(sock, msg, BUFSIZE, 0);
    msg[readlen]='\0';
}
void CommunicationManager::tcp_send_file(int fsock, char* filename) {
    char buffer[BUFSIZE];
    int readlen;
    FILE *fd = fopen(filename, "r");
    while (feof(fd) == 0) {
        readlen = fread(&buffer, sizeof(char), BUFSIZE-1, fd);
        send(fsock, buffer, readlen, 0);
        memset(buffer, 0, BUFSIZE);
    }
    tcp_close(fsock);
    fclose(fd);
}
void CommunicationManager::tcp_recv_file(int fsock, char* filename) {
    char buffer[BUFSIZE];
    FILE *fd = fopen(filename, "w");
    size_t datasize;
    int ind;
    memset(buffer, 0, BUFSIZE);
    while ((datasize = recv(fsock, buffer, BUFSIZE-1, 0)) > 0) {
        ind = fwrite(&buffer, 1, datasize, fd);
        if(ind < datasize) {
            printf("File write failed.\n");;
        }
        memset(buffer, 0, BUFSIZE);
    }
    tcp_close(fsock);
    fclose(fd);
}