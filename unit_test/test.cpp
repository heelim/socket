#include "common.h"
#include "CommunicationManager.h"

char buf[BUFSIZE];
int main(){
	pid_t pid;
	int sock, port;
	port = 8888;
	CommunicationManager test;
	pid = fork();
	if (pid > 0) {
		sock = test.tcp_listen(INADDR_ANY, port, 5);
		printf("server listening\n");

		int client_sock;
		struct sockaddr_in client_addr;
		int addr_len = sizeof(client_addr);
		client_sock = accept(sock, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len); 
		sprintf(buf, "client connected\n");
		send(client_sock, buf, strlen(buf), 0);
		close(client_sock);
		close(sock);
	}
	else if (pid == 0) {
		sock = test.tcp_connect("127.0.0.1", port);
		printf("client connected\n");
		int readlen;
		readlen = recv(sock, buf, BUFSIZE, 0);
		buf[readlen] = '\0';
		fprintf(stderr, "server:%s", buf);	
		close(sock);
	}
	else { 
		printf("fork failed\n");
	}

	return 0;
}

