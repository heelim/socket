#include "common.h"
 
 struct client_info {
 	int sock;
 	int fsock;
 	int room;
 	char ip[20];
 };

 #define MAXUSER 50
 #define MAXROOM 10
// #include "CommunicationManager.h"
class CommunicationManager {
public:
	// CommunicationManager();
	void set_ip(char*);
	void set_port(int);
	void set_fport(int);

	char* get_ip();
	int get_port();
	int get_fport();
	
	int tcp_listen(int, int, int);
	int tcp_connect(int);
	void tcp_close(int);
	void tcp_send_msg(int, char*);
	void tcp_recv_msg(int, char*);
	void tcp_send_file(int, char*);
	void tcp_recv_file(int, char*);
	
private:
	int num_user=0;
	int room_info[MAXROOM][MAXUSER]={0,};
	struct client_info client[MAXUSER];

	int port;
	int fport;

	// int sock, client_sock;
	// struct sockaddr_in client_addr;
	// int fsock; 

	// struct sockaddr_in addr;
	char ip_addr[16]; 
	// int port;
	// int sock;
	// int addrlen;	
	
};

