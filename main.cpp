#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int printAddr(struct sockaddr_in *);
int main(int argc, char* argv[]) {
	char *ip_addr = "127.0.0.1"; //localhost
	int port = 8888; //random port

	struct sockaddr_in sockAddr;

	sockAddr.sin_family = AF_INET;
	// sockAddr.sin_addr.s_addr = inet_addr(ip_addr);
	// inet_aton(ip_addr, &(sockAddr.sin_addr));
	inet_pton(AF_INET, ip_addr, &(sockAddr.sin_addr));
	sockAddr.sin_port = htons(port);
	printAddr(&sockAddr);
}
int printAddr(struct sockaddr_in *addr) {
	int port;
	char ip_addr[18];
	port = ntohs((*addr).sin_port);
	inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), ip_addr, sizeof(struct sockaddr_in));
	printf("IP: %s, Port: %d\n", ip_addr, port);
	return 0;
}