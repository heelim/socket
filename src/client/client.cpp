#include "common.h"
#include "CommunicationManager.h"

char rbuf[BUFSIZE];
char wbuf[BUFSIZE];
char buf[BUFSIZE];
CommunicationManager client;
void list_files(char*);
int main(int argc, char* argv[]) {
	char ip_addr[] = "127.0.0.1"; //server ip addr
	int port = 8888; //server port num
	int fport = 9999;
	int sock;
	sock = client.tcp_connect(ip_addr, port);
	if(sock < 0) return 0;
	int readlen;
	int maxfds;
	fd_set fds;

	char str[10];
	readlen=read(sock, str, 10-1);
	str[readlen] = '\0';
	fprintf(stderr, "Server connected, you are Client(%s)\n", str);

	maxfds = sock + 1;
	FD_ZERO(&fds);
	while(1) {
		fprintf(stderr, "ME: ");
		FD_SET(0, &fds);
		FD_SET(sock, &fds);
		if (select(maxfds, &fds, NULL, NULL, NULL) < 0){
			printf("select fail");
			exit(0);
		}
		if (FD_ISSET(sock, &fds)) {
			if ((readlen = recv(sock, rbuf, BUFSIZE, 0)) > 0) {
				rbuf[readlen] = '\0';
				fprintf(stderr, "\r%s", rbuf);	
			}
		}
		if (FD_ISSET(0, &fds)) {
			if (fgets(buf, BUFSIZE, stdin)) {
				if(strlen(buf)<2) {
					continue;
				}
				else if(strstr(buf, "/quit") != NULL) {
					client.tcp_close(sock);

					printf("Good bye.\n");
					exit(0);
				}
				if(strstr(buf, "/upload") != NULL) {
					list_files("./");
					printf("%s", wbuf);
					printf("\rto upload, input filename or type q to cancel : ");

					char filename[BUFSIZE];
					fgets(filename, BUFSIZE, stdin);
					int namelen = strlen(filename);

					if(namelen < 2 | strstr(filename, "q") != NULL) {
						continue;
					}
					sprintf(wbuf, "/upload %s\n", filename);
					filename[namelen-1]='\0';

					client.tcp_send_msg(sock, wbuf);

					int fsock;
					fsock = client.tcp_connect(ip_addr, fport);

					char buffer[BUFSIZE];
					client.tcp_recv_msg(fsock, buffer);
					printf("%s\n", buffer);

					client.tcp_send_file(fsock, filename);
					printf("file send @ client\n");
				}
				else if(!strncmp(buf, "/download", 9)) {
					char filename[BUFSIZE];
					int namelen = strlen(buf)-11;
					if(namelen<1) {
						printf("you should enter filename\n");
						continue;
					}
					strncpy(filename, buf+10, namelen);
					filename[namelen]='\0';

					sprintf(wbuf, "/download %s\n", filename);

					client.tcp_send_msg(sock, wbuf);

					int fsock = client.tcp_connect(ip_addr, fport);

					char buffer[BUFSIZE];
					client.tcp_recv_msg(fsock, buffer);
					printf("%s\n", buffer);

					client.tcp_recv_file(fsock, filename);
					printf("file receive @ client\n");			
				}
				else {
					if(buf[0]== '/') sprintf(wbuf, "%s\n", buf);
					else sprintf(wbuf, "Client(%s): %s\n", str, buf);
					client.tcp_send_msg(sock, wbuf);
				}
			}
		}
	}
	return 0;
}
void list_files(char* target_dir) {
	DIR *d;
	struct dirent *dir;
	bool flag=true;
	d = opendir(target_dir);
	if (d) {
		sprintf(wbuf, "available file list\n");
		while ((dir = readdir(d)) != NULL) {	
			if(dir->d_name[0]=='.') continue;
			sprintf(wbuf, "%s\t-%s\n", wbuf, dir->d_name);
			flag=false;
		}
		if(flag) sprintf(wbuf, "there is no file in directory\n");
		closedir(d);
	}
	// else sprintf(wbuf, "There is no available files\n");
}